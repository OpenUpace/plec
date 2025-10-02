#include <stdio.h>
#include "../include/lexer.h"

int main() {
    FILE *fp = fopen("test.txt", "r");
    if (!fp) {
        printf("File open error!\n");
        return 1;
    }

    while (1) {
        Token token = get_next_token(fp);
        printf("Token: type=%d, lexeme='%s'\n", token.type, token.lexeme);
        if (token.type == TOKEN_EOF) break;
    }

    fclose(fp);
    return 0;
}