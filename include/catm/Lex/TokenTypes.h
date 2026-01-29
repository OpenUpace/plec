#ifndef CATMC_TOKEN_TYPES_H
#define CATMC_TOKEN_TYPES_H

#include <unordered_map>
#include <string>
enum Token {
    tok_eof = -1,

    // commands
    tok_func = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5,

    tok_if = -6,
    tok_then = -7,
    tok_else = -8,
    tok_for = -9,
    tok_in = -10,
};

std::unordered_map<std::string, Token> keywords = {
    {"func", tok_func},
    {"extern", tok_extern},
    {"if", tok_if},
    {"then", tok_then},
    {"else", tok_else},
    {"for", tok_for},
    {"in", tok_in},
};

#endif