#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// 定义 Token 类型
enum class TokenType {
    // keywords
    INT,
    CHAR,
    FUNCTION,
    RETURN,
    MODULE,
    USE,
    PLUS,
    MINUS,
    MULTIPLICATION,
    DIVIDE,
    // other
    IDENTIFIER,
    NUMBER,
    UNICODE_CHAR,
    ZHCHAR,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    EQUAL,
    SEMICOLON,
    DOUBLE_QUOTE,
    COLON,
    COLON_COLON,
    QUESTION,
    AND,
    AT,
    LESS, // <
    GREATER, // >
    PERCENT,
    BACKSLASH,
    LBRACKET,
    RBRACKET,
    SINGLE_QUOTE,
    PIPE,
    EOF_TOKEN,
    UNKNOWN
};

// Token 结构体
struct Token {
    TokenType type;     // Token 的类型
    std::string value;  // Token 的原始字符串（比如 "fn", "main", "123"）
    int line;           // 行号（可选）
    int column;         // 列号（可选）

    Token()
            : type(TokenType::UNKNOWN), value(""), line(0), column(0) {}

    Token(const TokenType type, std::string value, const int line, const int column)
        : type(type), value(std::move(value)), line(line), column(column) {}
};

#endif //TOKEN_H
