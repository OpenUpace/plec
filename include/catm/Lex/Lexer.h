#ifndef CATMC_LEXER_H
#define CATMC_LEXER_H

#include "catm/Basic/SourceLocation.h"
#include "catm/Lex/Token.h"
#include <istream>
namespace catm {
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
} // namespace catm

#endif
