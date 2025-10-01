#include "../include/parser.h"
#include "../include/ast.h"

Parser::Parser(Lexer lexer) : 
    lexer(std::move(lexer)), currentToken(this->lexer.NextToken()) {}

Token Parser::Advance(Lexer l_token)
{
    currentToken = l_token.NextToken();
    return currentToken;
}

Token Parser::Peek() const
{
    return currentToken;
}

bool Parser::IsAtEnd() const {
    return currentToken.type == TokenType::EOF_TOKEN;
}

ASTNode* Parser::parseNumber()
{
    Token token = currentToken;
    if (token.type != NUMBER) return NULL;
}