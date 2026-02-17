#ifndef PLEC_LEXER_H
#define PLEC_LEXER_H

#include "ple/Basic/SourceLocation.h"
#include "ple/Lex/Token.h"
#include <istream>
namespace ple {
class Lexer {
  private:
    std::istream &input;
    SourceLocation CurLoc;
    int CurChar;

    void advance();
  public:
    explicit Lexer(std::istream &input) : input(input), CurChar(' '), CurLoc({1, 1, 0}) {}
    Token gettok();
};
} // namespace ple

#endif
