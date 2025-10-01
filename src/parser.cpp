#include "../include/parser.h"
#include "../include/ast.h"

Parser::Parser(Lexer lexer) : 
    lexer(std::move(lexer)), currentToken(this->lexer.NextToken()) {}

Token Parser::Advance()
{
    currentToken = lexer.NextToken();
    return currentToken;
}

Token Parser::Peek() const
{
    return currentToken;
}

bool Parser::IsAtEnd() const {
    return currentToken.type == TokenType::EOF_TOKEN;
}