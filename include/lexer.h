typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENT,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[64]; // or allocate dynamically for longer lexemes
} Token;

Token get_next_token(FILE *fp);