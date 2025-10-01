// lexer.cpp

#include "../include/lexer.h"

#include <iostream>

Lexer::Lexer(std::string src) : source(std::move(src)), position(0), line(1), column(1) {}

const static std::unordered_map<std::string, TokenType> keywords = {
    {"fn", TokenType::FUNCTION},
    {"return", TokenType::RETURN},
    {"int",TokenType::INT},
    {"char",TokenType::CHAR},
    {"module",TokenType::MODULE},
    {"use",TokenType::USE},
};

void Lexer::SkipWhitespace() {
    while (!IsAtEnd() && isspace(Peek())) {
        Advance();
    }
}

char Lexer::Advance() {
    const char32_t current = source[position++];
    if (current == '\n') {  // 换行符
        line++;
        column = 1;
    } else {
        column++;
    }
    return current;
}

char32_t Lexer::Peek() const {
    return source[position];
}

bool Lexer::IsAtEnd() const {
    return position >= source.length();
}

std::string Lexer::ReadIdentifier() {
    std::string result;
    while (!IsAtEnd() && (isalpha(Peek()) || isdigit(Peek()) || Peek() == '_')) {
        result += Advance();
    }
    return result;
}

std::string Lexer::ReadNumber() {
    std::string result;
    while (!IsAtEnd() && isdigit(Peek())) {
        result += Advance();
    }
    return result;
}

// 判断是否是汉字首字节（1110xxxx）
bool Lexer::IsChineseCharStart(const char32_t c) {
    return (c & 0xE0) == 0xE0; // 0xE0 = 11100000
}

// 吞掉完整UTF-8汉字（3字节）
Token Lexer::ExtractHanzi() {
    std::string zhchar;
    zhchar += source[position++]; // 吞第1字节
    zhchar += source[position++]; // 吞第2字节
    zhchar += source[position++]; // 吞第3字节
    return {TokenType::ZHCHAR, zhchar, line, column};
}

// 判断是否是有效UTF-8起始字节
bool IsUTF8StartByte(const unsigned char c) {
    if ((c & 0x80) == 0) return false;  // ASCII字符交给原有逻辑
    return (c & 0xC0) == 0xC0;          // 110xxxxx或111xxxxx
}

// 吞掉一个完整UTF-8字符（1-4字节）
Token Lexer::ExtractUTF8Char() {
    int len = 1;
    if ((source[position] & 0xF0) == 0xF0) len = 4;  // 11110xxx
    else if ((source[position] & 0xE0) == 0xE0) len = 3; // 1110xxxx
    else if ((source[position] & 0xC0) == 0xC0) len = 2; // 110xxxxx

    std::string utf8_char;
    for (int i = 0; i < len && !IsAtEnd(); ++i) {
        utf8_char += source[position++];
    }
    return {TokenType::UNICODE_CHAR, utf8_char, line, column};
}

Token Lexer::NextToken() {
    // 跳过空格、文件结尾
    SkipWhitespace();

    // 是否为文件结尾
    if (IsAtEnd()) {
        return {TokenType::EOF_TOKEN, "", line, column};
    }

    if (IsUTF8StartByte(source[position])) return ExtractUTF8Char();
    if (IsChineseCharStart(source[position])) return ExtractHanzi();

    char c = Peek();

    // 标识符或关键字
    if (isalpha(c) || c == '_') {
        std::string ident = ReadIdentifier();
        const auto it = keywords.find(ident);
        if (it != keywords.end()) {
            return {it->second, ident, line, column};
        }
        return {TokenType::IDENTIFIER, ident, line, column};
    }

    if (isdigit(c)) {
        std::string ident = ReadIdentifier();
        const auto it = keywords.find(ident);
        if (it != keywords.end()) {
            return {it->second, ident, line, column};
        }
    }

    // 数字
    if (isdigit(c)) {
        std::string number = ReadNumber();
        return {TokenType::NUMBER, number, line, column};
    }

    c = Advance();

    // 单符号识别
    switch (c) {
        case '+': return {TokenType::PLUS, "+", line, column};
        case '-': return {TokenType::MINUS, "-", line, column};
        case '*': return {TokenType::MULTIPLICATION, "*", line, column};
        case '/': return {TokenType::DIVIDE, "/", line, column};
        case '(': return {TokenType::LEFT_PAREN, "(", line, column};
        case ')': return {TokenType::RIGHT_PAREN, ")", line, column};
        case ',': return {TokenType::COMMA, ",", line, column};
        case ';': return {TokenType::SEMICOLON, ";", line, column};
        case ':': {
            if (Peek() == ':') {
                Advance(); return {TokenType::COLON_COLON, "::", line, column};
            }
            return {TokenType::COLON, ":", line, column};
        }
        case '.': return {TokenType::DOT, ".", line, column};
        case '[': return {TokenType::LBRACKET, "[", line, column};
        case ']': return {TokenType::RBRACKET, "]", line, column};
        case '{': return {TokenType::LEFT_BRACE, "{", line, column};
        case '}': return {TokenType::RIGHT_BRACE, "}", line, column};
        case '<': return {TokenType::LESS, "<", line, column};
        case '>': return {TokenType::GREATER, ">", line, column};
        case '%': return {TokenType::PERCENT, "%", line, column};
        case '\\': return {TokenType::BACKSLASH, "\\", line, column};
        case '\'': return {TokenType::SINGLE_QUOTE, "\'", line, column};
        case '\"': return {TokenType::DOUBLE_QUOTE, "\"", line, column};
        case '?': return {TokenType::QUESTION, "?", line, column};
        case '|': return {TokenType::PIPE, "|", line, column};
        case '&': return {TokenType::AND, "&", line, column};
        case '=': return {TokenType::EQUAL, "=", line, column};
        default: printf("[line %d, column %d]\033[31m error:\033[0m Unknown token: %c\n", line, column, c);
            return {TokenType::UNKNOWN, "", line, column};
    }

}
