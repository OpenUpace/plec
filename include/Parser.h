#ifndef PARSER_H
#define PARSER_H

// Parser.h
#pragma once

#include "Lexer.h"
#include "AST.h"
#include <memory>

class Parser {
public:
    explicit Parser(Lexer lexer);

    std::unique_ptr<Expr> Parse(); // 主入口，返回 AST 根节点

private:
    Token Advance();         // 消费当前 Token，获取下一个
    [[nodiscard]] Token Peek() const;            // 查看当前 Token
    bool Match(TokenType expectedType);   // 如果当前 Token 是目标类型，就前进
    [[nodiscard]] bool IsAtEnd() const;
    [[nodiscard]] bool Check(TokenType expectedType) const;

    // 表达式解析总入口
    std::unique_ptr<Expr> ParseExpression();

    // 赋值表达式（优先级最低）
    std::unique_ptr<Expr> ParseAssignment();

    // 逻辑或：a || b
    std::unique_ptr<Expr> ParseLogicalOr();

    // 逻辑与：a && b
    std::unique_ptr<Expr> ParseLogicalAnd();

    // 相等比较：a == b, a != b
    std::unique_ptr<Expr> ParseEquality();

    // 大小比较：a < b, a >= b
    std::unique_ptr<Expr> ParseComparison();

    // 加减运算：a + b, a - b
    std::unique_ptr<Expr> ParseTerm();

    // 乘除运算：a * b, a / b
    std::unique_ptr<Expr> ParseFactor();

    // 一元运算：-a, !b
    std::unique_ptr<Expr> ParseUnary();

    // 主表达式：字面量、变量、括号表达式
    std::unique_ptr<Expr> ParsePrimary();

    Lexer lexer;
    Token current;
};

#endif //PARSER_H
