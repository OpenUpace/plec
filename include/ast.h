#ifndef AST_H_
#define AST_H_

#include <lexer.h>

typedef enum { NUMBER, BINARY_OP } NodeType ;

typedef struct ASTNode {
    NodeType type;
    union {
        int number;
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            char op;
        } binary_op;
    } data;
} ASTNode;

ASTNode* createNumberCode(int value);
ASTNode* createBinanyOp(char op, ASTNode* left, ASTNode* right);
void printAST(ASTNode* node);

#endif