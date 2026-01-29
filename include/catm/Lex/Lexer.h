#ifndef CATMC_LEXER_H
#define CATMC_LEXER_H

#include "catm/Basic/SourceLocation.h"
#include <istream>
namespace catm {
class Lexer {
  private:
    std::istream &input;
    SourceLocation CurLoc{1, 1, 0};
    int CurChar = 0;

  public:
    explicit Lexer(std::istream &input) : input(input) {}
    void advance() {
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
    int gettok();
};
} // namespace catm

#endif