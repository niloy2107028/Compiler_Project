%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line_number = 1;
%}

%option noyywrap

%%

"পুন"      { printf("Token: INT\n"); }
"ভগ"          { printf("Token: FLOAT\n"); }
"যদি"              { printf("Token: IF\n"); }
"যদিনা"             { printf("Token: ELSE\n"); }
"যতক্ষণ"           { printf("Token: WHILE\n"); }
"লুপ"              { printf("Token: FOR\n"); }
"ইনপুট"            { printf("Token: INPUT\n"); }
"দেখাও"            { printf("Token: OUTPUT\n"); }
"শুরু"             { printf("Token: BEGIN\n"); }
"শেষ"              { printf("Token: END\n"); }

[০-৯]+                           { printf("Token: BANGLA_NUMBER (%s)\n", yytext); }
[0-9]+                           { printf("Token: NUMBER (%s)\n", yytext); }
[০-৯]+\.[০-৯]+                   { printf("Token: BANGLA_FLOAT (%s)\n", yytext); }
[0-9]+\.[0-9]+                   { printf("Token: FLOAT_NUM (%s)\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*           { printf("Token: IDENTIFIER (%s)\n", yytext); }
[অ-ৱ][অ-ৱ০-৯_]*                  { printf("Token: BANGLA_IDENTIFIER (%s)\n", yytext); }

"+"                { printf("Token: PLUS\n"); }
"-"                { printf("Token: MINUS\n"); }
"*"                { printf("Token: MULTIPLY\n"); }
"/"                { printf("Token: DIVIDE\n"); }
"="                { printf("Token: ASSIGN\n"); }
">"                { printf("Token: GREATER\n"); }
"<"                { printf("Token: LESS\n"); }
">="               { printf("Token: GREATER_EQUAL\n"); }
"<="               { printf("Token: LESS_EQUAL\n"); }
"=="               { printf("Token: EQUAL\n"); }
"!="               { printf("Token: NOT_EQUAL\n"); }

"("                { printf("Token: LPAREN\n"); }
")"                { printf("Token: RPAREN\n"); }
"{"                { printf("Token: LBRACE\n"); }
"}"                { printf("Token: RBRACE\n"); }
";"                { printf("Token: SEMICOLON\n"); }
","                { printf("Token: COMMA\n"); }

[ \t]+             { /* Ignore whitespace */ }
\n                 { line_number++; }
.                  { printf("Unknown character: %s\n", yytext); }

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("Cannot open file");
            return 1;
        }
        yyin = file;
    }
    
    printf("Starting lexical analysis...\n\n");
    while(yylex());
    printf("\nLexical analysis completed.\n");
    
    return 0;
}
