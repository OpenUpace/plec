/// IRGenerator.cpp - Some parts with Vibe Coding.
#include "ple/IRGenerator.h"
#include "CalcParser.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Constants.h"

using namespace llvm;

IRGenerator::IRGenerator(const std::string &ModuleName) {
    TheContext = std::make_unique<LLVMContext>();
    TheModule = std::make_unique<Module>(ModuleName, *TheContext);

    // Create a new builder for the module.
    Builder = std::make_unique<IRBuilder<>>(*TheContext);
}

std::any IRGenerator::visitProg(CalcParser::ProgContext *ctx) {
    FunctionType *FuncTYpe = FunctionType::get(Builder->getInt64Ty(), false);
    Function *MainFunc = Function::Create(FuncTYpe, Function::ExternalLinkage, "main", *TheModule);

    BasicBlock* entryBB = llvm::BasicBlock::Create(*TheContext, "entry", MainFunc);
    Builder->SetInsertPoint(entryBB);
    
    std::vector<llvm::Value*> results;
    for (auto* expr : ctx->expr()) {
        visit(expr);
        if (LastValue) {
            results.push_back(LastValue);
        }
    }
    
    if (!results.empty()) {
        llvm::Value* sum = Builder->getInt64(0);
        for (auto* val : results) {
            sum = Builder->CreateAdd(sum, val, "sumtmp");
        }
        Builder->CreateRet(sum);
    } else {
        Builder->CreateRet(Builder->getInt64(0));
    }
    
    llvm::verifyFunction(*MainFunc);
    return nullptr;
}

/// expr: (('+'|'-') term)*
std::any IRGenerator::visitExpr(CalcParser::ExprContext *ctx) {
    visit(ctx->term(0));
    llvm::Value* result = LastValue;
    
    for (size_t i = 1; i < ctx->term().size(); ++i) {
        char op = ctx->getStart()->getText()[0];
        visit(ctx->term(i));
        llvm::Value* right = LastValue;
        
        switch(op) {
            case '+':
                result = Builder->CreateAdd(result, right, "addtmp");
                break;
            case '-':
                result = Builder->CreateSub(result, right, "subtmp");
                break;
        }
    }
    
    LastValue = result;
    return result;
    return nullptr;
}

std::any IRGenerator::visitTerm(CalcParser::TermContext* ctx) {
    visit(ctx->factor(0));
    llvm::Value* result = LastValue;
    
    for (size_t i = 1; i < ctx->factor().size(); ++i) {
        char op = ctx->getStart()->getText()[0];
        visit(ctx->factor(i));
        llvm::Value* right = LastValue;
        
        switch(op) {
            case '*':
                result = Builder->CreateMul(result, right, "multmp");
                break;
            case '/':
                result = Builder->CreateSDiv(result, right, "divtmp");
                break;
        }
    }
    
    LastValue = result;
    return result;
}

std::any IRGenerator::visitFactor(CalcParser::FactorContext* ctx) {
    if (ctx->expr()) {
        // 括号表达式
        return visit(ctx->expr());
    } else if (ctx->INT()) {
        // 整数
        int value = std::stoi(ctx->INT()->getText());
        LastValue = ConstantInt::get(Type::getInt64Ty(*TheContext), llvm::APInt(64, value, true));
        return LastValue;
    }
    return nullptr;
}

void IRGenerator::printIR(void) {
    TheModule->print(errs(), nullptr);
}
