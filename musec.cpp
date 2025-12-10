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
/// gettok - Return the next token from the current input pointer.
static int gettok() {
        if (!CurPtr)
                return tok_eof;

        // Skip whitespace
        while (isspace(*CurPtr)) ++CurPtr;

        // Identifier: [a-zA-Z][a-zA-Z0-9]*
        if (isalpha(*CurPtr)) {
                IdentifierStr.clear();
                while (isalnum(*CurPtr)) {
                        IdentifierStr += *CurPtr;
                        ++CurPtr;
                }

                const auto it = keywords.find(IdentifierStr);
                if (it != keywords.end()) {
                        return it->second;
                }
                return tok_identifier;
        }

        // Number: [0-9.]+ (simple)
        if (isdigit(*CurPtr) || *CurPtr == '.') {
                std::string NumStr;
                while (isdigit(*CurPtr) || *CurPtr == '.') {
                        NumStr += *CurPtr;
                        ++CurPtr;
                }
                NumVal = strtod(NumStr.c_str(), 0);
                return tok_number;
        }

        // Comment: // ... to end of line
        if (*CurPtr == '/' && *(CurPtr + 1) == '/') {
                CurPtr += 2;
                while (*CurPtr && *CurPtr != '\n' && *CurPtr != '\r') ++CurPtr;
                if (!*CurPtr) return gettok();
                return gettok();
        }

        // End of file (string)
        if (*CurPtr == '\0')
                return tok_eof;

        // Otherwise, just return the character.
        int ThisChar = (unsigned char)*CurPtr;
        ++CurPtr;
        return ThisChar;
}

static int CurTok;
static int getNextToken(const char * s) { if (s) CurPtr = s; return CurTok = gettok(); }

static const char * tokenToString(int t)
{
        switch (t)
        {
        case tok_eof: return "EOF";
        case tok_int: return "int";
        case tok_return: return "return";
        case tok_pub: return "pub";
        case tok_module: return "module";
        case tok_unit: return "unit";
        case tok_use: return "use";
        case tok_identifier: return "Ident";
        case tok_number: return "Number";
        default: return "???";
        }
}

int main(int argc, char *argv[])
{
        const char* code = "pub unit program";

        // Print all tokens from the provided source string.
        auto printTokens = [](const char* src) {
                getNextToken(src);
                while (true) {
                        std::cout << "Token: " << tokenToString(CurTok) << std::endl;
                        if (CurTok == tok_eof)
                                break;
                        // Subsequent calls must not reinitialize the input pointer.
                        getNextToken(nullptr);
                }
        };

        printTokens(code);

        return 0;
}