#ifndef PARSER_H
#define PARSER_H

#include <lexer.h>
#include <ast.h>

class Parser {
public:
    explicit Parser(Lexer lexer);

private:
    Token Advance();                                // 消费当前 Token，获取下一个
    [[nodiscard]] Token Peek() const;               // 查看当前 Token
    bool Match(TokenType expectedType);             // 如果当前 Token 是目标类型，就前进
    [[nodiscard]] bool IsAtEnd() const;
    [[nodiscard]] bool Check(TokenType expectedType) const;

    ASTNode* paserExpression();
    ASTNode* parsePrimary();
    ASTNode* parseAdditiveExpr();
    ASTNode* parseMultiplicativeExpr();

    Lexer lexer;
    Token currentToken;
};

#endif
