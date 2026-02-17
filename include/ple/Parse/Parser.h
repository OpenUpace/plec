#ifndef PLEC_PARSER_H
#define PLEC_PARSER_H

#include "ple/Lex/Lexer.h"
#include "ple/AST/ASTForward.h"
#include <map>

namespace ple {
class Parser {
  public:
    explicit Parser(Lexer lexer) : lexer(lexer) {}
    Token CurTok;
    std::map<char, int> BinopPrecedence;

    std::unique_ptr<AST::ExprAST> LogError(const char *Str);
    std::unique_ptr<AST::PrototypeAST> LogErrorP(const char *Str);
    Token getNextToken();

    std::unique_ptr<AST::ExprAST> ParseExpression();
    std::unique_ptr<AST::ExprAST> ParseNumberExpr();
    std::unique_ptr<AST::ExprAST> ParseParenExpr();
    std::unique_ptr<AST::ExprAST> ParseIdentifierExpr();
    std::unique_ptr<AST::ExprAST> ParseIfExpr();
    std::unique_ptr<AST::ExprAST> ParseForExpr();
    std::unique_ptr<AST::ExprAST> ParsePrimary();
    std::unique_ptr<AST::ExprAST> ParseBinOpRHS(int ExprPrec,
                                           std::unique_ptr<AST::ExprAST> LHS);
    std::unique_ptr<AST::PrototypeAST> ParsePrototype();
    std::unique_ptr<AST::FunctionAST> ParseDefinition();
    std::unique_ptr<AST::FunctionAST> ParseTopLevelExpr();
    std::unique_ptr<AST::PrototypeAST> ParseExtern();

  private:
    Lexer lexer;
    int GetTokPrecedence();

};
} // namespace ple

#endif // !PLEC_PARSER_H
