#ifndef AST_H
#define AST_H

#include <stddef.h>

typedef enum
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_VOID,
    TYPE_ERROR
} ValueType;
// Data types of variables

typedef enum
{
    EXPR_LITERAL,
    EXPR_VARIABLE,
    EXPR_BINARY,
    EXPR_UNARY,
    EXPR_CALL
} ExprKind;

/* Types of expressions

Type	Example
LITERAL	5, 3.14
VARIABLE	x
BINARY	a + b
UNARY	-x
CALL	func() */

typedef enum
{
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_GT,
    OP_LT,
    OP_GE,
    OP_LE,
    OP_EQ,
    OP_NE,
    OP_NEG
} Operator;

typedef struct Expr Expr;
typedef struct Stmt Stmt;

typedef struct
{
    Expr **items;
    int count;
    int cap;
} ExprList;

struct Expr
{
    ExprKind kind;
    int line;
    union
    {
        struct
        {
            ValueType type;
            double number;
        } literal;
        char *var_name;
        struct
        {
            Operator op;
            Expr *left;
            Expr *right;
        } binary;
        struct
        {
            Operator op;
            Expr *expr;
        } unary;
        struct
        {
            char *name;
            ExprList args;
        } call;
    } as;
};

typedef enum
{
    STMT_BLOCK,
    STMT_DECL,
    STMT_ASSIGN,
    STMT_INPUT,
    STMT_OUTPUT,
    STMT_IF,
    STMT_WHILE,
    STMT_FOR,
    STMT_FUNC_DEF,
    STMT_RETURN,
    STMT_EXPR
} StmtKind;

typedef struct
{
    Stmt **items;
    int count;
    int cap;
} StmtList;

struct Stmt
{
    StmtKind kind;
    int line;
    union
    {
        StmtList block;
        struct
        {
            ValueType type;
            char *name;
            Expr *init;
        } decl;
        struct
        {
            char *name;
            Expr *value;
        } assign;
        struct
        {
            char *name;
        } input;
        struct
        {
            Expr *value;
        } output;
        struct
        {
            Expr *cond;
            Stmt *then_branch;
            Stmt *else_branch;
        } if_stmt;
        struct
        {
            Expr *cond;
            Stmt *body;
        } while_stmt;
        struct
        {
            Stmt *init;
            Expr *cond;
            Stmt *update;
            Stmt *body;
        } for_stmt;
        struct
        {
            char *name;
            ValueType return_type;
            Stmt *body;
        } func_def;
        struct
        {
            Expr *value;
        } return_stmt;
        struct
        {
            Expr *expr;
        } expr_stmt;
    } as;
};

ExprList expr_list_create(void);
void expr_list_append(ExprList *list, Expr *expr);

StmtList stmt_list_create(void);
void stmt_list_append(StmtList *list, Stmt *stmt);

Expr *ast_make_literal_int(int line, long value);
Expr *ast_make_literal_float(int line, double value);
Expr *ast_make_variable(int line, const char *name);
Expr *ast_make_binary(int line, Operator op, Expr *left, Expr *right);
Expr *ast_make_unary(int line, Operator op, Expr *expr);
Expr *ast_make_call(int line, const char *name, ExprList args);

Stmt *ast_make_block(int line, StmtList items);
Stmt *ast_make_decl(int line, ValueType type, const char *name, Expr *init);
Stmt *ast_make_assign(int line, const char *name, Expr *value);
Stmt *ast_make_input(int line, const char *name);
Stmt *ast_make_output(int line, Expr *value);
Stmt *ast_make_if(int line, Expr *cond, Stmt *then_branch, Stmt *else_branch);
Stmt *ast_make_while(int line, Expr *cond, Stmt *body);
Stmt *ast_make_for(int line, Stmt *init, Expr *cond, Stmt *update, Stmt *body);
Stmt *ast_make_func_def(int line, const char *name, ValueType return_type, Stmt *body);
Stmt *ast_make_return(int line, Expr *value);
Stmt *ast_make_expr_stmt(int line, Expr *expr);

void ast_free_expr(Expr *expr);
void ast_free_stmt(Stmt *stmt);

#endif
