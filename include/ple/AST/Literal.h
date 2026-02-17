#ifndef PLEC_LITERAL_H
#define PLEC_LITERAL_H

#include "ple/AST/Expr.h"
#include <string>

namespace ple {
namespace AST {
/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
    double Val;

  public:
    NumberExprAST(double Val) : Val(Val) {}
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
    std::string Name;

  public:
    VariableExprAST(const std::string &Name) : Name(Name) {}
};
} // namespace AST
} // namespace ple

#endif