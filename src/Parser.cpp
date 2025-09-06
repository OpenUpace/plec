#include "../include/Parser.h"

Parser::Parser(Lexer lexer)
    : lexer(std::move(lexer)), current(this->lexer.NextToken()) {}

Token Parser::Advance() {
    current = lexer.NextToken();
    return current;
}

Token Parser::Peek() const {
    return current;
}

bool Parser::Match(const TokenType expectedType) {
    if (Check(expectedType)) {
        Advance(); // 消耗当前 token
        return true;
    }
    return false;
}

bool Parser::Check(const TokenType expectedType) const {
    if (IsAtEnd()) return false;         // 文件结束了，就不能匹配任何 token
    return Peek().type == expectedType;  // 看看当前 token 类型是不是我们想要的
}

bool Parser::IsAtEnd() const {
    return current.type == TokenType::EOF_TOKEN;
}

std::unique_ptr<Expr> Parser::ParseExpression() {
    ;
}

std::unique_ptr<Expr> Parser::ParseAssignment() {
    ;
}

std::unique_ptr<Expr> Parser::ParsePrimary() {
    ;
}

