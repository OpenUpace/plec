#ifndef CATM_TOKEN_H
#define CATM_TOKEN_H

enum {
    tok_eof = -1,
    tok_func = -2,
    tok_extern = -3,
    tok_identifier = -4,
    tok_number = -5,
    tok_if = -6,
    tok_then = -7,
    tok_else = -8,
    tok_for = -9,
    tok_in = -10,
} Type;

#endif // Token.h
