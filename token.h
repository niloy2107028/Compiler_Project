#ifndef TOKEN_H
#define TOKEN_H

// Token types for the Bangla Programming Language Compiler

// Keywords
#define T_INT 1
#define T_FLOAT 2
#define T_IF 3
#define T_ELSE 4
#define T_WHILE 5
#define T_FOR 6
#define T_INPUT 7
#define T_OUTPUT 8
#define T_BEGIN 9
#define T_END 10

// Identifiers and literals
#define T_IDENTIFIER 20
#define T_BANGLA_IDENTIFIER 21
#define T_NUMBER 22
#define T_BANGLA_NUMBER 23
#define T_FLOAT_NUM 24
#define T_BANGLA_FLOAT 25

// Operators
#define T_PLUS 30
#define T_MINUS 31
#define T_MULTIPLY 32
#define T_DIVIDE 33
#define T_ASSIGN 34

// Comparison operators
#define T_GREATER 40
#define T_LESS 41
#define T_GREATER_EQUAL 42
#define T_LESS_EQUAL 43
#define T_EQUAL 44
#define T_NOT_EQUAL 45

// Delimiters
#define T_LPAREN 50
#define T_RPAREN 51
#define T_LBRACE 52
#define T_RBRACE 53
#define T_SEMICOLON 54
#define T_COMMA 55

// Token structure
typedef struct
{
    int type;
    char *lexeme;
    int line_number;
    union
    {
        int int_value;
        float float_value;
        char *string_value;
    } value;
} Token;

#endif // TOKEN_H
