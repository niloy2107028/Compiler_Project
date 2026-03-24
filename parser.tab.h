/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    BEGIN_KW = 258,                /* BEGIN_KW  */
    END_KW = 259,                  /* END_KW  */
    INT_KW = 260,                  /* INT_KW  */
    FLOAT_KW = 261,                /* FLOAT_KW  */
    IF_KW = 262,                   /* IF_KW  */
    ELSE_KW = 263,                 /* ELSE_KW  */
    WHILE_KW = 264,                /* WHILE_KW  */
    FOR_KW = 265,                  /* FOR_KW  */
    INPUT_KW = 266,                /* INPUT_KW  */
    OUTPUT_KW = 267,               /* OUTPUT_KW  */
    FUNCTION_KW = 268,             /* FUNCTION_KW  */
    RETURN_KW = 269,               /* RETURN_KW  */
    PLUS = 270,                    /* PLUS  */
    MINUS = 271,                   /* MINUS  */
    MULTIPLY = 272,                /* MULTIPLY  */
    DIVIDE = 273,                  /* DIVIDE  */
    ASSIGN = 274,                  /* ASSIGN  */
    GREATER = 275,                 /* GREATER  */
    LESS = 276,                    /* LESS  */
    GREATER_EQUAL = 277,           /* GREATER_EQUAL  */
    LESS_EQUAL = 278,              /* LESS_EQUAL  */
    EQUAL = 279,                   /* EQUAL  */
    NOT_EQUAL = 280,               /* NOT_EQUAL  */
    LPAREN = 281,                  /* LPAREN  */
    RPAREN = 282,                  /* RPAREN  */
    LBRACE = 283,                  /* LBRACE  */
    RBRACE = 284,                  /* RBRACE  */
    SEMICOLON = 285,               /* SEMICOLON  */
    COMMA = 286,                   /* COMMA  */
    LOWER_THAN_ELSE = 287,         /* LOWER_THAN_ELSE  */
    IDENTIFIER = 288,              /* IDENTIFIER  */
    INT_LITERAL = 289,             /* INT_LITERAL  */
    FLOAT_LITERAL = 290,           /* FLOAT_LITERAL  */
    UMINUS = 291                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 25 "parser.y"

    long int_val;        // integer value
    double float_val;    // float value
    char *str;           // identifier name
    Expr *expr;          // expression node
    Stmt *stmt;          // statement node
    StmtList stmt_list;  // list of statements
    ExprList expr_list;  // list of expressions
    ValueType type;      // type (int/float)

#line 111 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
