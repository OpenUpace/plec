#include "catm/Lex/Lexer.h"
#include "catm/Lex/TokenTypes.h"
#include <fstream>
#include <iostream>
#include <sstream>

static std::string IdentifierStr;
static double NumVal;

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
int catm::Lexer::gettok() {
    while (CurChar != EOF && isspace(CurChar))
        advance();

    if (CurChar != EOF &&
        isalpha(CurChar)) { // identifiter: [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = CurChar;
        advance();
        while (CurChar != EOF && isalnum(CurChar)) {
            IdentifierStr += CurChar;
            advance();
        }

        const auto it = keywords.find(IdentifierStr);
        if (it != keywords.end()) {
            return it->second;
        }
        return tok_identifier;
    }

    if (CurChar != EOF && isdigit(CurChar) ||
        CurChar == '.') { // Number: [0-9.]+
        std::string NumStr;
        do {
            NumStr += CurChar;
            advance();
        } while (CurChar != EOF && isdigit(CurChar) || CurChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    if (CurChar == '#') {
        do {
            advance();
        } while (CurChar != EOF && CurChar != '\n' && CurChar != 'r');

        if (CurChar != EOF)
            return gettok();
    }

    if (CurChar == EOF)
        return tok_eof;

    int ThisChar = CurChar;
    advance();
    return ThisChar;
}

int main(int argc, char *argv[]) {
    // std::ifstream file(argv[1]);
    std::string test_code = "for";
    std::istringstream file(test_code);
    catm::Lexer l(file);
    int tok;
    do {
        tok = l.gettok();
        std::cout << tok << std::endl;
    } while (tok != tok_eof);
}
