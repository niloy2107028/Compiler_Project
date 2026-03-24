#ifndef TOKEN_H
#define TOKEN_H

/*Prevents file from being included multiple times*/

#include "src/ast.h"

/* Includes AST definitions

So parser can use Expr, Stmt, etc. */

extern Stmt *g_program_ast;

/* Global AST variable

Final parsed program tree will be stored here
extern = defined in another file (your .y file) */

extern int line_number;

/* Tracks current line number

Used for error messages */

int yyparse(void);

/*  Main parser function

Starts parsing process */


int yylex(void);

/* Lexer function

Reads input and returns tokens */



#endif
