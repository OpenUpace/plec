#ifndef LEXER_H
#define LEXER_H

typedef enum {
    NUMBER,
    IDENT,
    KEYWORD,
    _EOF,
    UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[64]; // or allocate dynamically for longer lexemes
} Token;

Token get_next_token(FILE *fp);

#endif
