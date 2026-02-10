#ifndef CATMC_PARSER_H
#define CATMC_PARSER_H

#include "catm/Lex/Lexer.h"
#include "catm/Parse/ASTNode.h"
#include <map>

namespace catm {
class Parser {
  public:
    explicit Parser(Lexer lexer) : lexer(lexer) {}
    Token CurTok;
    std::map<char, int> BinopPrecedence;

    std::unique_ptr<ExprAST> LogError(const char *Str);
    std::unique_ptr<catm::PrototypeAST> LogErrorP(const char *Str);
    Token getNextToken();

    std::unique_ptr<ExprAST> ParseExpression();
    std::unique_ptr<ExprAST> ParseNumberExpr();
    std::unique_ptr<ExprAST> ParseParenExpr();
    std::unique_ptr<ExprAST> ParseIdentifierExpr();
    std::unique_ptr<ExprAST> ParseIfExpr();
    std::unique_ptr<ExprAST> ParseForExpr();
    std::unique_ptr<ExprAST> ParsePrimary();
    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                           std::unique_ptr<ExprAST> LHS);
    std::unique_ptr<PrototypeAST> ParsePrototype();
    std::unique_ptr<FunctionAST> ParseDefinition();
    std::unique_ptr<FunctionAST> ParseTopLevelExpr();
    std::unique_ptr<PrototypeAST> ParseExtern();

  private:
    Lexer lexer;
    int GetTokPrecedence();

};
} // namespace catm

#endif // !CATMC_PARSER_H
