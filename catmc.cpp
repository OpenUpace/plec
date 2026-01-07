/*
 * Muse Complier - Powered by LLVM Kaleidoscope
 */
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <unordered_map>

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
        tok_eof = -1,

        // commands
        tok_int = -2,
        tok_return = -3,
        tok_pub = -4,
        tok_module = -5,
        tok_unit = -6,
        tok_use = -7,

        // primary
        tok_identifier = -8,
        tok_number = -9
};

static std::unordered_map<std::string, Token> keywords =
{
        {"int", tok_int},
        {"return", tok_return},
        {"pub", tok_pub},
        {"module", tok_module},
        {"unit", tok_unit},
        {"use", tok_use}
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number

static const char* CurPtr = nullptr;

// TODO: AI-Generated, needs refactoring.
// - Remove lambda for clarity.
// - Revert gettok to take source as parameter for better reentrancy.
/// gettok - Return the next token from standard input.
static int gettok() {
        static int LastChar = ' ';

        // Skip any whitespace.
        while (isspace(LastChar))
                LastChar = getchar();

        if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
                IdentifierStr = LastChar;
                while (isalnum((LastChar = getchar())))
                IdentifierStr += LastChar;

                const auto it = keywords.find(IdentifierStr);
                if (it != keywords.end()) {
                        return it->second;
                }
                return tok_identifier;
        }

        if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
                std::string NumStr;
                do {
                        NumStr += LastChar;
                        LastChar = getchar();
                } while (isdigit(LastChar) || LastChar == '.');

                NumVal = strtod(NumStr.c_str(), nullptr);
                return tok_number;
        }

        if (LastChar == '#') {
                // Comment until end of line.
                do
                        LastChar = getchar();
                while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

                if (LastChar != EOF)
                return gettok();
        }

        // Check for end of file.  Don't eat the EOF.
        if (LastChar == EOF)
                return tok_eof;

        // Otherwise, just return the character as its ascii value.
        int ThisChar = LastChar;
        LastChar = getchar();
        return ThisChar;
}

int main(int argc, char *argv[])
{
        // const char* code = "pub unit program";
        int tok;
        do
        {
                tok = gettok();
                std::cout << tok << std::endl;
        } while (tok != tok_eof);
        

        return 0;
}