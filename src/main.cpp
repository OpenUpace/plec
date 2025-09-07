#include <iostream>
#include <sstream>
#include <fstream>
#include "../include/Lexer.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << argv[0] << "\033[31m fatal error: \033[0m" << "no input files" << std::endl;
        std::cerr << "compilation terminated." << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << argv[0] << "\033[31m error: \033[0m" << argv[1] << ": No such file or directory" << std::endl;
        std::cerr << "compilation terminated." << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    Lexer lexer(source);
    Token token;

    do {
        token = lexer.NextToken();
        std::cout << token_type_to_string(token.type) << std::endl;
    } while (token.type != TokenType::EOF_TOKEN);

    return 0;
}