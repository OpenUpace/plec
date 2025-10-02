#ifndef _LEXER_H
#define _LEXER_H

#include <stddef.h>

typedef enum
{
    NUMBER,
    INENT,
    EOF
} TokenType;

typedef struct
{
    TokenType type;
    const char* value;
    int line;
    int column;
} Token;

typedef struct Lexer Lexer;

struct Lexer
{
    Token nextToken();

    void SkipWhitespace();
    char Advance();
    char Peek();

    const char* source;
    size_t position;
    int line;
    int column;
};


#endif