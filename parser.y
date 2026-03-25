%{
/*   C CODE SECTION   */

#include <stdio.h>
#include <stdlib.h>

#include "token.h"   // contains AST structures and helper functions

/* Global AST root */
Stmt *g_program_ast = NULL;

/* Lexer function (from .l file) */
int yylex(void);

/* Error handler */
void yyerror(const char *s);
%}

/* Enable location tracking (line numbers etc.) */
%locations

/*   DATA TYPES   */

/* Union to store different types of values */
%union {
    long int_val;        // integer value
    double float_val;    // float value
    char *str;           // identifier name
    Expr *expr;          // expression node
    Stmt *stmt;          // statement node
    StmtList stmt_list;  // list of statements
    ExprList expr_list;  // list of expressions
    ValueType type;      // type (int/float)
}

/*   TOKENS   */

/* Keywords */
%token BEGIN_KW END_KW INT_KW FLOAT_KW IF_KW ELSE_KW WHILE_KW FOR_KW INPUT_KW OUTPUT_KW
%token FUNCTION_KW RETURN_KW

/* Operators */
%token PLUS MINUS MULTIPLY DIVIDE ASSIGN
%token GREATER LESS GREATER_EQUAL LESS_EQUAL EQUAL NOT_EQUAL

/* Symbols */
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA

/* Special precedence helper */
%token LOWER_THAN_ELSE

/* Tokens with values */
%token <str> IDENTIFIER
%token <int_val> INT_LITERAL
%token <float_val> FLOAT_LITERAL

/*   NON-TERMINAL TYPES   */

%type <stmt> program statement block declaration assignment input_stmt output_stmt if_stmt while_stmt for_stmt func_def return_stmt
%type <stmt_list> stmt_list
%type <expr> expr
%type <expr_list> arg_list opt_arg_list
%type <type> type_spec

/*   PRECEDENCE   */

%left EQUAL NOT_EQUAL GREATER LESS GREATER_EQUAL LESS_EQUAL
%left PLUS MINUS
%left MULTIPLY DIVIDE

%precedence UMINUS           // unary minus (-x)
%precedence LOWER_THAN_ELSE  // helps solve if-else ambiguity
%precedence ELSE_KW

%%

/*   GRAMMAR RULES   */

/* Program must start with BEGIN and end with END */
program
    : BEGIN_KW stmt_list END_KW
      {
        /* Create AST block node */
        g_program_ast = ast_make_block(@1.first_line, $2);
        $$ = g_program_ast;
      }
    ;

/* List of statements */
stmt_list
    : %empty
      {
        /* Empty list */
        $$ = stmt_list_create();
      }
    | stmt_list statement
      {
        $$ = $1;
        if ($2) {
            stmt_list_append(&$$, $2); // add statement to list
        }
      }
    ;

/* Different types of statements */
statement
    : declaration SEMICOLON { $$ = $1; }
    | assignment SEMICOLON { $$ = $1; }
    | input_stmt SEMICOLON { $$ = $1; }
    | output_stmt SEMICOLON { $$ = $1; }
    | if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | for_stmt { $$ = $1; }
    | func_def { $$ = $1; }
    | return_stmt SEMICOLON { $$ = $1; }
    | block { $$ = $1; }

    /* Expression as statement */
    | expr SEMICOLON {
        $$ = ast_make_expr_stmt(@1.first_line, $1);
      }

    /* Error recovery */
    | error SEMICOLON
      {
        yyerror("Recovered from syntax error");
        yyerrok;
        $$ = NULL;
      }
    ;

/* Block (nested program) */
block
    : BEGIN_KW stmt_list END_KW
      {
        $$ = ast_make_block(@1.first_line, $2);
      }
    ;

/* Type specification */
type_spec
    : INT_KW { $$ = TYPE_INT; }
    | FLOAT_KW { $$ = TYPE_FLOAT; }
    ;

/* Variable declaration */
declaration
    : type_spec IDENTIFIER
      {
        $$ = ast_make_decl(@2.first_line, $1, $2, NULL);
        free($2); // free identifier memory
      }
    | type_spec IDENTIFIER ASSIGN expr
      {
        $$ = ast_make_decl(@2.first_line, $1, $2, $4);
        free($2);
      }
    ;

/* Assignment */
assignment
    : IDENTIFIER ASSIGN expr
      {
        $$ = ast_make_assign(@1.first_line, $1, $3);
        free($1);
      }
    ;

/* Input statement */
input_stmt
    : INPUT_KW IDENTIFIER
      {
        $$ = ast_make_input(@2.first_line, $2);
        free($2);
      }
    ;

/* Output statement */
output_stmt
    : OUTPUT_KW expr
      {
        $$ = ast_make_output(@1.first_line, $2);
      }
    ;

/* If statement */
if_stmt
    : IF_KW LPAREN expr RPAREN statement %prec LOWER_THAN_ELSE
      {
        /* if without else */
        $$ = ast_make_if(@1.first_line, $3, $5, NULL);
      }
    | IF_KW LPAREN expr RPAREN statement ELSE_KW statement
      {
        /* if with else */
        $$ = ast_make_if(@1.first_line, $3, $5, $7);
      }
    ;

/* While loop */
while_stmt
    : WHILE_KW LPAREN expr RPAREN statement
      {
        $$ = ast_make_while(@1.first_line, $3, $5);
      }
    ;

/* For loop */
for_stmt
    : FOR_KW LPAREN assignment SEMICOLON expr SEMICOLON assignment RPAREN statement
      {
        $$ = ast_make_for(@1.first_line, $3, $5, $7, $9);
      }
    ;

/* Function definition */
func_def
    : FUNCTION_KW type_spec IDENTIFIER LPAREN RPAREN block
      {
        $$ = ast_make_func_def(@1.first_line, $3, $2, $6);
        free($3);
      }
    ;

/* Return statement */
return_stmt
    : RETURN_KW expr
      {
        $$ = ast_make_return(@1.first_line, $2);
      }
    ;

/*   EXPRESSIONS   */

expr
    : expr PLUS expr { $$ = ast_make_binary(@2.first_line, OP_ADD, $1, $3); }
    | expr MINUS expr { $$ = ast_make_binary(@2.first_line, OP_SUB, $1, $3); }
    | expr MULTIPLY expr { $$ = ast_make_binary(@2.first_line, OP_MUL, $1, $3); }
    | expr DIVIDE expr { $$ = ast_make_binary(@2.first_line, OP_DIV, $1, $3); }

    /* Comparisons */
    | expr GREATER expr { $$ = ast_make_binary(@2.first_line, OP_GT, $1, $3); }
    | expr LESS expr { $$ = ast_make_binary(@2.first_line, OP_LT, $1, $3); }
    | expr GREATER_EQUAL expr { $$ = ast_make_binary(@2.first_line, OP_GE, $1, $3); }
    | expr LESS_EQUAL expr { $$ = ast_make_binary(@2.first_line, OP_LE, $1, $3); }
    | expr EQUAL expr { $$ = ast_make_binary(@2.first_line, OP_EQ, $1, $3); }
    | expr NOT_EQUAL expr { $$ = ast_make_binary(@2.first_line, OP_NE, $1, $3); }

    /* Unary minus */
    | MINUS expr %prec UMINUS { $$ = ast_make_unary(@1.first_line, OP_NEG, $2); }

    /* Parentheses */
    | LPAREN expr RPAREN { $$ = $2; }

    /* Function call */
    | IDENTIFIER LPAREN opt_arg_list RPAREN
      {
        $$ = ast_make_call(@1.first_line, $1, $3);
        free($1);
      }

    /* Variable */
    | IDENTIFIER
      {
        $$ = ast_make_variable(@1.first_line, $1);
        free($1);
      }

    /* Literals */
    | INT_LITERAL
      {
        $$ = ast_make_literal_int(@1.first_line, $1);
      }
    | FLOAT_LITERAL
      {
        $$ = ast_make_literal_float(@1.first_line, $1);
      }
    ;

/* Optional argument list */
opt_arg_list
    : %empty
      {
        $$ = expr_list_create();
      }
    | arg_list { $$ = $1; }
    ;

/* Argument list */
arg_list
    : expr
      {
        $$ = expr_list_create();
        expr_list_append(&$$, $1);
      }
    | arg_list COMMA expr
      {
        $$ = $1;
        expr_list_append(&$$, $3);
      }
    ;

%%

/*   ERROR FUNCTION   */

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", line_number, s);
}