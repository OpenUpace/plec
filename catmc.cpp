/*
 * Muse Complier - Powered by LLVM Kaleidoscope
 */
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Scalar/LoopSimplifyCFG.h"
#include "KaleidoscopeJIT.h"

using namespace llvm;
using namespace llvm::orc;

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
        tok_eof = -1,

        // commands
        tok_int = -2,
        tok_return = -3,
        tok_pub = -4,
        tok_module = -5,
        tok_unit = -6,
        tok_use = -7,
        tok_func = -8,
        tok_extern = -9,

        // primary
        tok_identifier = -10,
        tok_number = -11
};

static std::unordered_map<std::string, Token> keywords =
{
        {"int", tok_int},
        {"return", tok_return},
        {"pub", tok_pub},
        {"module", tok_module},
        {"unit", tok_unit},
        {"use", tok_use},
        {"func", tok_func},
        {"extern", tok_extern},
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number

/// gettok - Return the next token from standard input.
static int gettok() {
        static int LastChar = ' ';

        // Skip any whitespace.
        while (isspace(LastChar))
                LastChar = getchar();

        if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
                IdentifierStr = LastChar;
                while (isalnum((LastChar = getchar())))
                IdentifierStr += LastChar;

                const auto it = keywords.find(IdentifierStr);
                if (it != keywords.end()) {
                        return it->second;
                }
                return tok_identifier;
        }

        if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
                std::string NumStr;
                do {
                        NumStr += LastChar;
                        LastChar = getchar();
                } while (isdigit(LastChar) || LastChar == '.');

                NumVal = strtod(NumStr.c_str(), nullptr);
                return tok_number;
        }

        if (LastChar == '#') {
                // Comment until end of line.
                do
                        LastChar = getchar();
                while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

                if (LastChar != EOF)
                return gettok();
        }

        // Check for end of file.  Don't eat the EOF.
        if (LastChar == EOF)
                return tok_eof;

        // Otherwise, just return the character as its ascii value.
        int ThisChar = LastChar;
        LastChar = getchar();
        return ThisChar;
}

// === AST
class ExprAST {
public:
        virtual ~ExprAST() = default;
        virtual Value *codegen() = 0;
};

class NumberExprAST : public ExprAST {
        double Val;
public:
        NumberExprAST(double Val):Val(Val){};
        Value *codegen() override;
};

class VariableExprAST : public ExprAST {
        std::string Name;
public:
        VariableExprAST(const std::string& Name):Name(Name){};
        Value *codegen() override;
};

class BinaryExprAST : public ExprAST {
        char Op;
        std::unique_ptr<ExprAST> LHS, RHS;
public:
        BinaryExprAST(char Op,std::unique_ptr<ExprAST> LHS,std::unique_ptr<ExprAST> RHS):
                Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)){};
        Value *codegen() override;
};

class CallExprAST : public ExprAST {
        std::string Callee;
        std::vector<std::unique_ptr<ExprAST>> Args;
public:
        CallExprAST(const std::string& Callee, std::vector<std::unique_ptr<ExprAST>> Args):
                Callee(Callee), Args(std::move(Args)){}
        Value *codegen() override;
};

class PrototypeAST : public ExprAST {
        std::string Name;
        std::vector<std::string> Args;
public:
        PrototypeAST(const std::string& Name, std::vector<std::string> Args):
                Name(Name), Args(std::move(Args)){}
        Function *codegen() override;
        const std::string &getName() const { return Name; }
};

class FunctionAST {
        std::unique_ptr<PrototypeAST> Proto;
        std::unique_ptr<ExprAST> Body;
public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body):
                Proto(std::move(Proto)), Body(std::move(Body)){}
        Function *codegen();
};

// === Parser
static int CurTok;
static int getNextToken() { return CurTok = gettok(); }

static std::map<char, int> BinopPrecedence;

static int GetTokPrecedence()
{
        if (!isascii(CurTok)) return -1;

        int TokPrec = BinopPrecedence[CurTok];
        if (TokPrec <= 0) return -1;
        return TokPrec;
}

std::unique_ptr<ExprAST> LogError(const char* Str)
{
        fprintf(stderr, "Error: %s\n", Str);
        return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char* Str)
{
        LogError(Str);
        return nullptr;
}

static std::unique_ptr<ExprAST> ParseExpression();

// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr()
{
        auto Result = std::make_unique<NumberExprAST>(NumVal);
        getNextToken();
        return std::move(Result);
}

// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr()
{
        getNextToken(); // eat (.
        auto V = ParseExpression();
        if (!V) return nullptr;

        if (CurTok != ')') return LogError("expexted ')'");
        getNextToken(); // eat ).
        return V;
}

// identifierexpr
//      ::= identifier
//      ::= identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr() {
        std::string IdName = IdentifierStr;

        getNextToken(); // eat identifier.

        if (CurTok != '(')
                return std::make_unique<VariableExprAST>(IdName);

        // Call.
        getNextToken(); // eat (.
        std::vector<std::unique_ptr<ExprAST>> Args;
        if (CurTok != ')')
        {
                while (true)
                {
                        if (auto Arg = ParseExpression())
                                Args.push_back(std::move(Arg));
                        else return nullptr;

                        if (CurTok == ')') break;

                        if (CurTok != ',')
                                return LogError("Exprcted ')' or ',' in argment list");
                        getNextToken();
                }
        }

        // Eat the ')'.
        getNextToken();

        return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

// primary
//   ::= identifierexpr
//   ::= numberexpr
//   ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary() {
        switch (CurTok) {
                default:
                        return LogError("unknown token when expecting an expression");
                case tok_identifier:
                        return ParseIdentifierExpr();
                case tok_number:
                        return ParseNumberExpr();
                case '(':
                        return ParseParenExpr();
        }
}

// binoprhs
//      ::= ('+' primary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprRrec, std::unique_ptr<ExprAST> LHS)
{
        while (true)
        {
                int TokPrec = GetTokPrecedence();

                if (TokPrec < ExprRrec) return LHS;

                int BinOp = CurTok;
                getNextToken(); // eat binop

                auto RHS = ParsePrimary();
                if (!RHS) return nullptr;

                int NextPrec = GetTokPrecedence();
                if (TokPrec < NextPrec)
                {
                        RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
                        if (!RHS) return nullptr;
                }

                LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
        }
}

static std::unique_ptr<ExprAST> ParseExpression()
{
        auto LHS = ParsePrimary();
        if (!LHS) return nullptr;

        return ParseBinOpRHS(0, std::move(LHS));
}

// prototype
//      ::= id '(' id* ')'
static std::unique_ptr<PrototypeAST> ParsePrototype()
{
        if (CurTok != tok_identifier)
                return LogErrorP("Expected function name in prototype");
        
        std::string FnName = IdentifierStr;
        getNextToken();

        if (CurTok != '(')
                return LogErrorP("Expected '(' in prototype");
        
        std::vector<std::string> ArgNames;
        while (getNextToken() == tok_identifier)
                ArgNames.push_back(IdentifierStr);
        if (CurTok != ')')
                return LogErrorP("Expected ')' in prototype");
        
        // success.
        getNextToken(); // eat ')'.

        return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

// definition ::= 'func' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition()
{
        getNextToken(); // eat func.
        auto Proto = ParsePrototype();
        if (!Proto) return nullptr;

        if (auto E = ParseExpression())
                return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
        return nullptr;
}

// toplevelexpr ::= expression
static std::unique_ptr<FunctionAST> ParseTopLevelExpr() {
        if (auto E = ParseExpression()) {
                // Make an anonymous proto.
                auto Proto = std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string>());
                return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
        }
        return nullptr;
}

// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern() {
        getNextToken(); // eat extern.
        return ParsePrototype();
}

// === Codegen
static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<IRBuilder<>> Builder;
static std::unique_ptr<Module> TheModule;
static std::map<std::string, Value *> NameValues;
static std::unique_ptr<KaleidoscopeJIT> TheJIT;
static std::unique_ptr<FunctionPassManager> TheFPM;
static std::unique_ptr<LoopAnalysisManager> TheLAM;
static std::unique_ptr<FunctionAnalysisManager> TheFAM;
static std::unique_ptr<CGSCCAnalysisManager> TheCGAM;
static std::unique_ptr<ModuleAnalysisManager> TheMAM;
static std::unique_ptr<PassInstrumentationCallbacks> ThePIC;
static std::unique_ptr<StandardInstrumentations> TheSI;
static std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;
static ExitOnError ExitOnErr;

Value *LogErrorV(const char* Str)
{
        LogError(Str);
        return nullptr;
}

Function *getFunction(std::string Name)
{
        if (auto *F = TheModule->getFunction(Name)) return F;

        auto FI = FunctionProtos.find(Name);
        if (FI != FunctionProtos.end()) return FI->second->codegen();

        return nullptr;
}

Value *NumberExprAST::codegen()
{
        return ConstantFP::get(*TheContext, APFloat(Val));
}

Value *VariableExprAST::codegen()
{
        Value *V = NameValues[Name];
        if (!V)
                return LogErrorV("Unknown variable name");
        return V;
}

Value *BinaryExprAST::codegen()
{
        Value *L = LHS->codegen();
        Value *R = RHS->codegen();

        if (!L||!R) return nullptr;

        switch (Op)
        {
        case '+':
                return Builder->CreateFAdd(L, R, "addtmp");
        case '-':
                return Builder->CreateFSub(L, R, "subtmp");
        case '*':
                return Builder->CreateFMul(L, R, "multmp");
        case '<':
                L = Builder->CreateFCmpULT(L, R, "cmptmp");
                return Builder->CreateUIToFP(L, Type::getDoubleTy(*TheContext), "booltmp");
        default:
                return LogErrorV("invalid binary operator");
        }
}

Value *CallExprAST::codegen()
{
        Function *CalleeF = getFunction(Callee);
        if (!CalleeF) return LogErrorV("Unknown function referenced");
        if (CalleeF->arg_size() != Args.size()) return LogErrorV("Incorrect # argments passed");

        std::vector<Value *> ArgsV;
        for (unsigned i = 0, e = Args.size(); i != e; ++i)
        {
                Value *ArgVal = Args[i]->codegen();
                if (!ArgVal) return nullptr;
                ArgsV.push_back(ArgVal);
        }
        return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

Function *PrototypeAST::codegen()
{
        std::vector<Type *> Doubles(Args.size(), Type::getDoubleTy(*TheContext));
        FunctionType *FT = FunctionType::get(Type::getDoubleTy(*TheContext), Doubles, false);
        Function *F = Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());

        unsigned Idx = 0;
        for (auto& Arg : F->args())
                Arg.setName(Args[Idx++]);
        return F;
}

Function *FunctionAST::codegen()
{
        auto &P = *Proto;
        FunctionProtos[Proto->getName()] = std::move(Proto);
        Function *TheFunction = getFunction(P.getName());
        if (!TheFunction) return nullptr;

        BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
        Builder->SetInsertPoint(BB);

        NameValues.clear();
        for (auto& Arg : TheFunction->args())
                NameValues[std::string(Arg.getName())] = &Arg;

        if (Value *RetVal = Body->codegen())
        {
                Builder->CreateRet(RetVal);

                verifyFunction(*TheFunction);

                TheFPM->run(*TheFunction, *TheFAM);

                return TheFunction;
        }
        TheFunction->eraseFromParent();
        return nullptr;
}

// === Top-level

static void InitializeModuleAndManagers(void)
{
        TheContext = std::make_unique<LLVMContext>();
        TheModule = std::make_unique<Module>("KaleidoscopeJIT", *TheContext);
        Builder = std::make_unique<IRBuilder<>>(*TheContext);

        TheFPM = std::make_unique<FunctionPassManager>();
        TheLAM = std::make_unique<LoopAnalysisManager>();
        TheFAM = std::make_unique<FunctionAnalysisManager>();
        TheCGAM = std::make_unique<CGSCCAnalysisManager>();
        TheMAM = std::make_unique<ModuleAnalysisManager>();
        ThePIC = std::make_unique<PassInstrumentationCallbacks>();
        TheSI = std::make_unique<StandardInstrumentations>(*TheContext, true);

        TheSI->registerCallbacks(*ThePIC, TheMAM.get());
        TheFPM->addPass(InstCombinePass());
        TheFPM->addPass(ReassociatePass());
        TheFPM->addPass(GVNPass());
        TheFPM->addPass(createFunctionToLoopPassAdaptor(LoopSimplifyCFGPass()));

        PassBuilder PB;
        PB.registerModuleAnalyses(*TheMAM);
        PB.registerFunctionAnalyses(*TheFAM);
        PB.crossRegisterProxies(*TheLAM, *TheFAM, *TheCGAM, *TheMAM);
}

static void HandleDefinition()
{
        if (auto FnAST = ParseDefinition())
        {
                if (auto *FnIR = FnAST->codegen())
                {
                        fprintf(stderr, "Read function definition:");
                        FnIR->print(errs());
                        fprintf(stderr, "\n");
                        ExitOnErr(TheJIT->addModule(ThreadSafeModule(std::move(TheModule), std::move(TheContext))));
                        InitializeModuleAndManagers();
                }
        } else {
                getNextToken();
        }
}

static void HandleExtern()
{
        if (auto ProtoAST = ParseExtern())
        {
                if (auto *FnIR = ProtoAST->codegen())
                {
                        fprintf(stderr, "Read extern:");
                        FnIR->print(errs());
                        fprintf(stderr, "\n");
                        FunctionProtos[ProtoAST->getName()] = std::move(ProtoAST);
                }
        } else {
                getNextToken();
        }
}

static void HandleTopLevelExpression()
{
        if (auto FnAST = ParseTopLevelExpr())
        {
                if (FnAST->codegen()) {
                        // Create a ResourceTracker to track JIT'd memory allocated to our
                        // anonymous expression -- that way we can free it after executing.
                        auto RT = TheJIT->getMainJITDylib().createResourceTracker();

                        auto TSM = ThreadSafeModule(std::move(TheModule), std::move(TheContext));
                        ExitOnErr(TheJIT->addModule(std::move(TSM), RT));
                        InitializeModuleAndManagers();

                        // Search the JIT for the __anon_expr symbol.
                        auto ExprSymbol = ExitOnErr(TheJIT->lookup("__anon_expr"));

                        // Get the symbol's address and cast it to the right type (takes no
                        // arguments, returns a double) so we can call it as a native function.
                        double (*FP)() = ExprSymbol.getAddress().toPtr<double (*)()>();
                        fprintf(stderr, "Evaluated to %f\n", FP());

                        // Delete the anonymous expression module from the JIT.
                        ExitOnErr(RT->remove());
                }
        } else {
                getNextToken();
        }
}

static void MainLoop()
{
        while (true)
        {
                fprintf(stderr, "ready> ");
                switch (CurTok)
                {
                case tok_eof:
                        return;
                case ';':
                        getNextToken();
                        break;
                case tok_func:
                        HandleDefinition();
                        break;
                case tok_extern:
                        HandleExtern();
                        break;
                default:
                        HandleTopLevelExpression();
                        break;
                }
        }
        
}

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

/// putchard - putchar that takes a double and returns 0.
extern "C" DLLEXPORT double putchard(double X) {
        fputc((char)X, stderr);
        return 0;
}

/// printd - printf that takes a double prints it as "%f\n", returning 0.
extern "C" DLLEXPORT double printd(double X) {
        fprintf(stderr, "%f\n", X);
        return 0;
}

int main(int argc, char *argv[])
{
        InitializeNativeTarget();
        InitializeNativeTargetAsmPrinter();
        InitializeNativeTargetAsmParser();
        BinopPrecedence['<'] = 10;
        BinopPrecedence['+'] = 20;
        BinopPrecedence['-'] = 20;
        BinopPrecedence['*'] = 40; // highest.

        fprintf(stderr, "ready> ");
        getNextToken();

        TheJIT = ExitOnErr(KaleidoscopeJIT::Create());

        InitializeModuleAndManagers();

        MainLoop();

        return 0;
}