#ifndef ERROR_H
#define ERROR_H

#pragma once
#include  <map>
#include <vector>
#include "Lexer.h"
#include "parser.h"

enum class ErrorLevel {
    WARNING,
    ERROR,
    FATAL
};

class ErrorReporter {
public:
    ErrorReporter();

    void ReportLexerError(int line, int column, const std::string& message);
    void ReportParserError(int line, int column, const std::string& message);

    [[nodiscard]] bool HasErrors() const;
    [[nodiscard]] int GetErrorCount() const;

    void Clear();
    void PrintAllErrors() const;

private:
    std::map<std::string, int> errors;
    std::vector<std::string> errorMessages;
    int errorCount = 0;
};


#endif //ERROR_H