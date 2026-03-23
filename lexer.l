%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fileno(FILE *stream);

#include "src/ast.h"
#include "parser.tab.h"

int line_number = 1;

static long parse_bangla_int(const char *text);
static double parse_bangla_float(const char *text);
static int bangla_digit_at(const char *s, int *digit, int *bytes);

static char *dup_text(const char *text) {
    size_t len = strlen(text) + 1;
    char *copy = (char *)malloc(len);
    if (!copy) {
        exit(1);
    }
    memcpy(copy, text, len);
    return copy;
}
%}

%option noyywrap
%option nounput
%option noinput

BENG_CHAR   ((\xE0\xA6[\x80-\xBF])|(\xE0\xA7[\x80-\xBF]))
BENG_DIGIT  (\xE0\xA7[\xA6-\xAF])

%%

"পুন"               { return INT_KW; }
"ভগ"               { return FLOAT_KW; }
"যদি"               { return IF_KW; }
"যদিনা"             { return ELSE_KW; }
"যতক্ষণ"             { return WHILE_KW; }
"লুপ"               { return FOR_KW; }
"ইনপুট"             { return INPUT_KW; }
"দেখাও"             { return OUTPUT_KW; }
"শুরু"              { return BEGIN_KW; }
"শেষ"               { return END_KW; }
"func"              { return FUNCTION_KW; }
"return"            { return RETURN_KW; }

">="                { return GREATER_EQUAL; }
"<="                { return LESS_EQUAL; }
"=="                { return EQUAL; }
"!="                { return NOT_EQUAL; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return MULTIPLY; }
"/"                 { return DIVIDE; }
"="                 { return ASSIGN; }
">"                 { return GREATER; }
"<"                 { return LESS; }

"("                 { return LPAREN; }
")"                 { return RPAREN; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }

({BENG_DIGIT})+\.({BENG_DIGIT})+      {
                        yylval.float_val = parse_bangla_float(yytext);
                        return FLOAT_LITERAL;
                     }
[0-9]+\.[0-9]+      {
                        yylval.float_val = strtod(yytext, NULL);
                        return FLOAT_LITERAL;
                     }
({BENG_DIGIT})+              {
                        yylval.int_val = parse_bangla_int(yytext);
                        return INT_LITERAL;
                     }
[0-9]+              {
                        yylval.int_val = strtol(yytext, NULL, 10);
                        return INT_LITERAL;
                     }

[a-zA-Z_][a-zA-Z0-9_]*  {
                           yylval.str = dup_text(yytext);
                           return IDENTIFIER;
                        }
{BENG_CHAR}({BENG_CHAR}|{BENG_DIGIT}|[0-9_])* {
                           if (strcmp(yytext, "\xE0\xA6\xAB\xE0\xA6\xBE\xE0\xA6\x82\xE0\xA6\xB6\xE0\xA6\xA8") == 0 ||
                               strcmp(yytext, "\xE0\xA6\xAB\xE0\xA6\xBE\xE0\xA6\x82\xE0\xA6\xB7\xE0\xA6\xA8") == 0 ||
                               strcmp(yytext, "\xE0\xA6\xAB\xE0\xA6\xBE\xE0\xA6\x82\xE0\xA6\xB8\xE0\xA6\xA8") == 0) {
                               return FUNCTION_KW;
                           }
                           if (strcmp(yytext, "\xE0\xA6\xAB\xE0\xA7\x87\xE0\xA6\xB0\xE0\xA6\xA4") == 0) {
                               return RETURN_KW;
                           }
                           yylval.str = dup_text(yytext);
                           return IDENTIFIER;
                        }

[ \t\r]+            { }
\n                  { line_number++; }
.                   {
                        fprintf(stderr, "Lexical error (line %d): invalid token '%s'\n", line_number, yytext);
                     }

%%

static int bangla_digit_at(const char *s, int *digit, int *bytes) {
    if (strncmp(s, "০", strlen("০")) == 0) { *digit = 0; *bytes = (int)strlen("০"); return 1; }
    if (strncmp(s, "১", strlen("১")) == 0) { *digit = 1; *bytes = (int)strlen("১"); return 1; }
    if (strncmp(s, "২", strlen("২")) == 0) { *digit = 2; *bytes = (int)strlen("২"); return 1; }
    if (strncmp(s, "৩", strlen("৩")) == 0) { *digit = 3; *bytes = (int)strlen("৩"); return 1; }
    if (strncmp(s, "৪", strlen("৪")) == 0) { *digit = 4; *bytes = (int)strlen("৪"); return 1; }
    if (strncmp(s, "৫", strlen("৫")) == 0) { *digit = 5; *bytes = (int)strlen("৫"); return 1; }
    if (strncmp(s, "৬", strlen("৬")) == 0) { *digit = 6; *bytes = (int)strlen("৬"); return 1; }
    if (strncmp(s, "৭", strlen("৭")) == 0) { *digit = 7; *bytes = (int)strlen("৭"); return 1; }
    if (strncmp(s, "৮", strlen("৮")) == 0) { *digit = 8; *bytes = (int)strlen("৮"); return 1; }
    if (strncmp(s, "৯", strlen("৯")) == 0) { *digit = 9; *bytes = (int)strlen("৯"); return 1; }
    return 0;
}

static long parse_bangla_int(const char *text) {
    long value = 0;
    const char *p = text;
    while (*p) {
        int digit = 0;
        int consumed = 0;
        if (!bangla_digit_at(p, &digit, &consumed)) {
            break;
        }
        value = value * 10 + digit;
        p += consumed;
    }
    return value;
}

static double parse_bangla_float(const char *text) {
    const char *dot = strchr(text, '.');
    long whole = 0;
    long frac = 0;
    long scale = 1;
    const char *p = text;
    while (p < dot) {
        int digit = 0;
        int consumed = 0;
        if (!bangla_digit_at(p, &digit, &consumed)) {
            break;
        }
        whole = whole * 10 + digit;
        p += consumed;
    }
    p = dot ? dot + 1 : text;
    while (*p) {
        int digit = 0;
        int consumed = 0;
        if (!bangla_digit_at(p, &digit, &consumed)) {
            break;
        }
        frac = frac * 10 + digit;
        scale *= 10;
        p += consumed;
    }
    return (double)whole + (double)frac / (double)scale;
}
