#ifndef PLEC_OPERATION_H
#define PLEC_OPERATION_H

#include "ple/AST/Expr.h"
#include <memory>

namespace ple {
namespace AST {
/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

  public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                  std::unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};
} // namespace AST
} // namespace ple

#endif