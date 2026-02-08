#include "catm/Lex/Lexer.h"

/// --
namespace catm {
std::unordered_map<std::string, Type> keywords = {
    {"func", tok_func}, {"extern", tok_extern}, {"if", tok_if},
    {"then", tok_then}, {"else", tok_else},     {"for", tok_for},
    {"in", tok_in},
};
}

/// Lexer

void catm::Lexer::advance() {
    CurChar = input.get();
    if (CurChar != EOF) {
        CurLoc.offset++;
        if (CurChar == '\n') {
            CurLoc.line++;
            CurLoc.column = 1;
        } else
            CurLoc.column++;
    } else
        CurChar = EOF;
}

/// @brief Return the next token from standard input.
/// @return Next token.
catm::Token catm::Lexer::gettok() {
    Token tok;
    tok.Loc = CurLoc;
    while (CurChar != EOF && isspace(CurChar))
        advance();

    if (CurChar != EOF &&
        isalpha(CurChar)) { // identifiter: [a-zA-Z][a-zA-Z0-9]*
        tok.IdentifierStr = CurChar;
        advance();
        while (CurChar != EOF && isalnum(CurChar)) {
            tok.IdentifierStr += CurChar;
            advance();
        }

        const auto it = keywords.find(tok.IdentifierStr);
        if (it != keywords.end()) {
            tok.type = it->second;
            return tok;
        }
        tok.type = tok_identifier;
        return tok;
    }

    if (CurChar != EOF && isdigit(CurChar) ||
        CurChar == '.') { // Number: [0-9.]+
        std::string NumStr;
        do {
            NumStr += CurChar;
            advance();
        } while (CurChar != EOF && isdigit(CurChar) || CurChar == '.');

        tok.NumVal = strtod(NumStr.c_str(), nullptr);
        tok.type = tok_number;
        return tok;
    }

    if (CurChar == '#') {
        do {
            advance();
        } while (CurChar != EOF && CurChar != '\n' && CurChar != '\r');

        if (CurChar != EOF)
            return gettok();
    }

    if (CurChar == EOF) {
        tok = tok_eof;
        return tok;
    }

    int ThisChar = CurChar;
    tok.type = ThisChar;
    advance();
    return tok;
}

