%{
/*  C CODE SECTION  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function to get file descriptor (used internally sometimes) */
int fileno(FILE *stream);

/* Include AST and parser header (for tokens like INT_KW, etc.) */
#include "src/ast.h"
#include "parser.tab.h"

/* Track line number for error reporting */
int line_number = 1;

/* Function declarations (defined later) */
static long parse_bangla_int(const char *text);
static double parse_bangla_float(const char *text);
static int bangla_digit_at(const char *s, int *digit, int *bytes);

/* Duplicate string safely (used for identifiers) */
static char *dup_text(const char *text) {
    size_t len = strlen(text) + 1;
    char *copy = (char *)malloc(len);   // allocate memory
    if (!copy) {
        exit(1); // exit if memory allocation fails
    }
    memcpy(copy, text, len); // copy text
    return copy;
}
%}

/*  FLEX OPTIONS  */

%option noyywrap   // no need for yywrap()
%option nounput    // disable unused functions
%option noinput

/*  REGEX DEFINITIONS  */

/* Bangla character (Unicode range) */
BENG_CHAR   ((\xE0\xA6[\x80-\xBF])|(\xE0\xA7[\x80-\xBF]))

/* Bangla digit (০-৯) */
BENG_DIGIT  (\xE0\xA7[\xA6-\xAF])

%%

/*  KEYWORDS  */

/* Bangla keywords mapped to tokens */
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
"ফাংশন"               { return FUNCTION_KW; }
"ফেরত"               { return RETURN_KW; }

/* English keywords */
"func"              { return FUNCTION_KW; }
"return"            { return RETURN_KW; }

/*  OPERATORS  */

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

/*  SYMBOLS  */

"("                 { return LPAREN; }
")"                 { return RPAREN; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }

/*  NUMBERS  */

/* Bangla float (e.g., ১২.৩৪) */
({BENG_DIGIT})+\.({BENG_DIGIT})+      {
    yylval.float_val = parse_bangla_float(yytext); // convert to float
    return FLOAT_LITERAL;
}

/* English float (e.g., 12.34) */
[0-9]+\.[0-9]+      {
    yylval.float_val = strtod(yytext, NULL); // convert string to double
    return FLOAT_LITERAL;
}

/* Bangla integer (e.g., ১২৩) */
({BENG_DIGIT})+ {
    yylval.int_val = parse_bangla_int(yytext); // convert Bangla digits
    return INT_LITERAL;
}

/* English integer */
[0-9]+ {
    yylval.int_val = strtol(yytext, NULL, 10);
    return INT_LITERAL;
}

/*  IDENTIFIERS  */

/* English identifiers (variable names) */
[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = dup_text(yytext); // store identifier
    return IDENTIFIER;
}

/* Bangla identifiers */
{BENG_CHAR}({BENG_CHAR}|{BENG_DIGIT}|[0-9_])* {

    /* Otherwise treat as identifier */
    yylval.str = dup_text(yytext);
    return IDENTIFIER;
}

/*  WHITESPACE & NEWLINES  */

[ \t\r]+    { }   // ignore spaces, tabs

/*\r = carriage return → moves cursor to start of line*/


\n          { line_number++; }  // increase line count

/*  ERROR HANDLING  */

. {
    fprintf(stderr, "Lexical error (line %d): invalid token '%s'\n",
            line_number, yytext);
}

%%

/*  HELPER FUNCTIONS  */

/* Check if current text starts with a Bangla digit */
static int bangla_digit_at(const char *s, int *digit, int *bytes) {

    /*  "১" → digit = 1 */

    /* Match each Bangla digit and return its numeric value */
    if (strncmp(s, "০", strlen("০")) == 0) { *digit = 0; *bytes = strlen("০"); return 1; }
    if (strncmp(s, "১", strlen("১")) == 0) { *digit = 1; *bytes = strlen("১"); return 1; }
    if (strncmp(s, "২", strlen("২")) == 0) { *digit = 2; *bytes = strlen("২"); return 1; }
    if (strncmp(s, "৩", strlen("৩")) == 0) { *digit = 3; *bytes = strlen("৩"); return 1; }
    if (strncmp(s, "৪", strlen("৪")) == 0) { *digit = 4; *bytes = strlen("৪"); return 1; }
    if (strncmp(s, "৫", strlen("৫")) == 0) { *digit = 5; *bytes = strlen("৫"); return 1; }
    if (strncmp(s, "৬", strlen("৬")) == 0) { *digit = 6; *bytes = strlen("৬"); return 1; }
    if (strncmp(s, "৭", strlen("৭")) == 0) { *digit = 7; *bytes = strlen("৭"); return 1; }
    if (strncmp(s, "৮", strlen("৮")) == 0) { *digit = 8; *bytes = strlen("৮"); return 1; }
    if (strncmp(s, "৯", strlen("৯")) == 0) { *digit = 9; *bytes = strlen("৯"); return 1; }

    return 0; // not a Bangla digit
}

/* Convert Bangla integer string → normal integer */
static long parse_bangla_int(const char *text) {

        /*  "১২৩" → 123 */


    long value = 0;
    const char *p = text;

    while (*p) {
        int digit = 0;
        int consumed = 0;

        /* Get digit and number of bytes consumed */
        if (!bangla_digit_at(p, &digit, &consumed)) {
            break;
        }

        value = value * 10 + digit; // build number
        p += consumed; // move pointer
    }

    return value;
}

/* Convert Bangla float string → double */
static double parse_bangla_float(const char *text) {

    const char *dot = strchr(text, '.'); // find decimal point

    long whole = 0; // integer part
    long frac = 0;  // fractional part
    long scale = 1; // divisor

    const char *p = text;

    /* Parse integer part */
    while (p < dot) {
        int digit = 0, consumed = 0;

        if (!bangla_digit_at(p, &digit, &consumed)) break;

        whole = whole * 10 + digit;
        p += consumed;
    }

    /* Parse fractional part */
    p = dot ? dot + 1 : text;

    while (*p) {
        int digit = 0, consumed = 0;

        if (!bangla_digit_at(p, &digit, &consumed)) break;

        frac = frac * 10 + digit;
        scale *= 10;
        p += consumed;
    }

    /* Combine integer + fraction */
    return (double)whole + (double)frac / (double)scale;
}