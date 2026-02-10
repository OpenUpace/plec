#include "catm/Parse/Parser.h"
#include <iostream>

using namespace catm;
static Lexer lexer(std::cin);
static Parser parser(lexer);


static void HandleDefinition() {
    if (parser.ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        // Skip token for error recovery.
        parser.getNextToken();
    }
}

static void HandleExtern() {
    if (parser.ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        // Skip token for error recovery.
        parser.getNextToken();
    }
}

static void HandleTopLevelExpression() {
    // Evaluate a top-level expression into an anonymous function.
    if (parser.ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        // Skip token for error recovery.
        parser.getNextToken();
    }
}

/// top ::= definition | external | expression | ';'
static void MainLoop() {
    while (true) {
        fprintf(stderr, "ready> ");
        switch (parser.CurTok.type) {
        case tok_eof:
            return;
        case ';': // ignore top-level semicolons.
            parser.getNextToken();
            break;
        case tok_func:
            HandleDefinition();
            break;
        case tok_extern:
            HandleExtern();
            break;
        default:
            HandleTopLevelExpression();
            break;
        }
    }
}

int main() {
    // Install standard binary operators.
    // 1 is lowest precedence.
    parser.BinopPrecedence['<'] = 10;
    parser.BinopPrecedence['+'] = 20;
    parser.BinopPrecedence['-'] = 20;
    parser.BinopPrecedence['*'] = 40; // highest.

           // Prime the first token.
    fprintf(stderr, "ready> ");
    parser.getNextToken();

           // Run the main "interpreter loop" now.
    MainLoop();

    return 0;
}
