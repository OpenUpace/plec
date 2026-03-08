#include <fstream>
#include <iostream>
#include "antlr4-runtime.h"
#include "CalcLexer.h"
#include "CalcParser.h"
#include "ple/IRGenerator.h"

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: plec <file>" << std::endl;
        return 0;
    }
    std::ifstream stream;
    stream.open(argv[1]);
    antlr4::ANTLRInputStream input(stream);
    CalcLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    CalcParser parser(&tokens);
    auto tree = parser.prog();
    
    IRGenerator gen("Calc");

    gen.visitProg(tree);

    gen.printIR();
    return 0;
}