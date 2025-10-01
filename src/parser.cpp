#include "../include/parser.h"

Parser::Parser(Lexer lexer) : lexer(std::move(lexer)), currentToken(this->lexer.NextToken()) {}