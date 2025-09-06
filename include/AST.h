#ifndef AST_H
#define AST_H

#pragma once
#include <string>
#include <memory>

enum class ExprType {
    NUMBER_LITERAL,
    IDENTIFIER,
    BINARY_EXPR
};

class Expr {
public:
    virtual ~Expr() = default;
    [[nodiscard]]virtual ExprType GetType() const = 0;
};

class NumberExpr : public Expr {
public:
    explicit NumberExpr(std::string val) : value(std::move(val)) {}
    [[nodiscard]]ExprType GetType() const override { return ExprType::NUMBER_LITERAL; }

    std::string value;
};

class BinaryExpr : public Expr {
public:
    BinaryExpr(std::unique_ptr<Expr> left, std::string op, std::unique_ptr<Expr> right)
        : lhs(std::move(left)), op(std::move(op)), rhs(std::move(right)) {}

    [[nodiscard]]ExprType GetType() const override { return ExprType::BINARY_EXPR; }

    std::unique_ptr<Expr> lhs;
    std::string op;
    std::unique_ptr<Expr> rhs;
};

#endif //AST_H
