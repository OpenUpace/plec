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
    const char current = source[position++];
    if (current == '\n') {  // 换行符
        line++;
        column = 1;
    } else {
        column++;
    }
    return current;
}

char Lexer::Peek() const {
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

Token Lexer::NextToken() {
    // 跳过空格、文件结尾
    SkipWhitespace();

    // 是否为文件结尾
    if (IsAtEnd()) {
        return {TokenType::EOF_TOKEN, "", line, column};
    }

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

    // 元符号：例@，!
    if (isalpha(c) || c == '@' || c == '!') {
        c = Advance();
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

std::string token_type_to_string(const TokenType type) {
    switch (type) {
        case TokenType::FUNCTION: return "FN";
        case TokenType::RETURN: return "RETURN";
        case TokenType::INT: return "INT";
        case TokenType::CHAR: return "CHAR";
        case TokenType::MODULE: return "MODULE";
        case TokenType::USE: return "USE";
        case TokenType::COLON: return "COLON";
        case TokenType::COLON_COLON: return "COLON_COLON";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLICATION: return "MULTIPLICATION";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::EOF_TOKEN: return "EOF";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "???";
    }
}
