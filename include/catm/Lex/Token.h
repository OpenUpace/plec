#ifndef CATM_TOKEN_H
#define CATM_TOKEN_H

#include "catm/Basic/SourceLocation.h"
#include <string>
#include <unordered_map>

namespace catm {
enum Type {
    tok_eof = -1,
    tok_unknown = -2,
    tok_func = -3,
    tok_extern = -4,
    tok_identifier = -5,
    tok_number = -6,
    tok_if = -7,
    tok_then = -8,
    tok_else = -9,
    tok_for = -10,
    tok_in = -11,
};

struct Token {
    int type;
    std::string IdentifierStr;
    double NumVal;
    SourceLocation Loc;

    Token(Type t = tok_unknown,
        const std::string& id = "",
        double num = 0.0,
        SourceLocation l = {1, 1, 0})
      : type(t), IdentifierStr(id), NumVal(num), Loc(l) {}
};

extern std::unordered_map<std::string, Type> keywords;
}
#endif // Token.h
