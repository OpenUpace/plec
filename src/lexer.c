#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "../include/lexer.h"

// Checks if a string is a keyword
int is_keyword(const char *str) {
    return strcmp(str, "number") == 0;
}

Token get_next_token(FILE *fp) {
    int c;
    Token token = {TOKEN_UNKNOWN, ""};

    // Skip whitespace
    while ((c = fgetc(fp)) != EOF && isspace(c));

    if (c == EOF) {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "eof");
        return token;
    }

    // Number
    if (isdigit(c)) {
        int i = 0;
        token.lexeme[i++] = c;
        while ((c = fgetc(fp)) != EOF && isdigit(c)) {
            if (i < sizeof(token.lexeme)-1)
                token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        token.type = TOKEN_NUMBER;

        // Unget the non-digit character
        if (c != EOF) ungetc(c, fp);
        return token;
    }

    // Identifier or Keyword (starts with letter or '_')
    if (isalpha(c) || c == '_') {
        int i = 0;
        token.lexeme[i++] = c;
        while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_')) {
            if (i < sizeof(token.lexeme)-1)
                token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';

        if (is_keyword(token.lexeme))
            token.type = TOKEN_KEYWORD;
        else
            token.type = TOKEN_IDENT;

        // Unget the non-ident character
        if (c != EOF) ungetc(c, fp);
        return token;
    }

    // Otherwise, unknown token
    token.lexeme[0] = c;
    token.lexeme[1] = '\0';
    token.type = TOKEN_UNKNOWN;
    return token;
}