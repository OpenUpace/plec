#pragma once
// Token and lexer headers

#include <string>
#include "token.h"
#include <unordered_map>
#include <stdint.h>

std::string token_type_to_string(TokenType type);

class Lexer {
public:
    explicit Lexer(std::string src);

    Token NextToken(); // 外部调用，获取下一个 Token

private:
    void SkipWhitespace();
    // void SkipComment();
    char Advance();
    [[nodiscard]] char32_t Peek() const;
    // [[nodiscard]] char PeekNext() const;
    [[nodiscard]] bool IsAtEnd() const;

    std::string ReadIdentifier();
    std::string ReadNumber();

    Token ExtractHanzi();
    Token ExtractUTF8Char();
    bool IsChineseCharStart(char32_t c);

    std::string source;
    size_t position;
    int line;
    int column;
};
