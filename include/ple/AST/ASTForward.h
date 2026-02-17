#ifndef PLEC_AST_FORWARD_H
#define PLEC_AST_FORWARD_H

#include "ple/AST/Expr.h"
#include "ple/AST/ControlFlow.h"
#include "ple/AST/Function.h"
#include "ple/AST/Literal.h"
#include "ple/AST/Operation.h"
#include <variant>

namespace ple {
namespace AST {
class ExprAST;
class NumberExprAST;
class VariableExprAST;
class BinaryExprAST;
class IfExprAST;
class ForExprAST;
class CallExprAST;
class PrototypeAST;
class FunctionAST;

using Expr = std::variant<NumberExprAST, VariableExprAST, BinaryExprAST, IfExprAST, ForExprAST, CallExprAST, PrototypeAST, FunctionAST>;
} // namespace AST
} // namespace ple

#endif // !PLEC_AST_NODE_H
