#include "../include/ast.h"
#include <stdio.h>
#include <stdlib.h>

ASTNode* createNumberCode(int value)
{
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NUMBER;
    node->data.number = value;
    return node;
}

ASTNode* createBinanyOp(char op, ASTNode* left, ASTNode* right)
{
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = BINARY_OP;
    node->data.binary_op.op = op;
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;
    return node;
}

void printAST(ASTNode* node)
{
    if (node == NULL) return;

    switch (node->type)
    {
    case NUMBER:
        printf("%d ", node->data.number);
        break;
    case BINARY_OP:
        printAST(node->data.binary_op.left);
        printAST(node->data.binary_op.right);
        printf("%c ", node->data.binary_op.op);
    }
}