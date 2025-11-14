%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *yytext;
extern FILE * yyin;
%}

number              [0-9]
ident               [a-zA-Z_]

%%
"int"               {printf("INT");}
{number}+           {printf("NUM: %s",yytext);}
{ident}+            {printf("IDENT: %s",yytext);}
%%

int yywrap() {
    return 1;
}

int main() {
    yylex();
    return 0;
}
