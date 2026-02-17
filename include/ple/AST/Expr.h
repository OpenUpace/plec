#ifndef PLEC_EXPR_H
#define PLEC_EXPR_H

namespace ple {
namespace AST {
/// ExprAST - Base class for all expression nodes.
class ExprAST {
  public:
    virtual ~ExprAST() = default;
};
} // namespace AST
} // namespace ple

#endif