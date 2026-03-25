#include "ast.h"

#include <stdlib.h>
#include <string.h>

static void *xcalloc(size_t n, size_t size) {
    void *p = calloc(n, size);
    if (!p) {
        exit(1);
    }
    return p;
}

static char *xstrdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = (char *)xcalloc(len, 1);
    memcpy(copy, s, len);
    return copy;
}

ExprList expr_list_create(void) {
    ExprList list;
    list.items = NULL;
    list.count = 0;
    list.cap = 0;
    return list;
}

void expr_list_append(ExprList *list, Expr *expr) {
    if (list->count == list->cap) {
        int new_cap = list->cap == 0 ? 4 : list->cap * 2;
        list->items = (Expr **)realloc(list->items, sizeof(Expr *) * (size_t)new_cap);
        list->cap = new_cap;
    }
    list->items[list->count++] = expr;
}

StmtList stmt_list_create(void) {
    StmtList list;
    list.items = NULL;
    list.count = 0;
    list.cap = 0;
    return list;
}

void stmt_list_append(StmtList *list, Stmt *stmt) {
    if (list->count == list->cap) {
        int new_cap = list->cap == 0 ? 8 : list->cap * 2;
        list->items = (Stmt **)realloc(list->items, sizeof(Stmt *) * (size_t)new_cap);
        list->cap = new_cap;
    }
    list->items[list->count++] = stmt;
}

static Expr *alloc_expr(int line, ExprKind kind) {
    Expr *e = (Expr *)xcalloc(1, sizeof(Expr));
    e->line = line;
    e->kind = kind;
    return e;
}

static Stmt *alloc_stmt(int line, StmtKind kind) {
    Stmt *s = (Stmt *)xcalloc(1, sizeof(Stmt));
    s->line = line;
    s->kind = kind;
    return s;
}

Expr *ast_make_literal_int(int line, long value) {
    Expr *e = alloc_expr(line, EXPR_LITERAL);
    e->as.literal.type = TYPE_INT;
    e->as.literal.number = (double)value;
    return e;
}

Expr *ast_make_literal_float(int line, double value) {
    Expr *e = alloc_expr(line, EXPR_LITERAL);
    e->as.literal.type = TYPE_FLOAT;
    e->as.literal.number = value;
    return e;
}

Expr *ast_make_variable(int line, const char *name) {
    Expr *e = alloc_expr(line, EXPR_VARIABLE);
    e->as.var_name = xstrdup(name);
    return e;
}

static int is_literal_expr(const Expr *expr) {
    return expr && expr->kind == EXPR_LITERAL;
}

Expr *ast_make_binary(int line, Operator op, Expr *left, Expr *right) {
    if (is_literal_expr(left) && is_literal_expr(right)) {
        double l = left->as.literal.number;
        double r = right->as.literal.number;
        ValueType out_type = (left->as.literal.type == TYPE_FLOAT || right->as.literal.type == TYPE_FLOAT)
            ? TYPE_FLOAT
            : TYPE_INT;
        double result = 0.0;
        int is_foldable = 1;
        switch (op) {
            case OP_ADD: result = l + r; break;
            case OP_SUB: result = l - r; break;
            case OP_MUL: result = l * r; break;
            case OP_DIV: result = r == 0.0 ? 0.0 : l / r; out_type = TYPE_FLOAT; break;
            case OP_GT: result = l > r; out_type = TYPE_BOOL; break;
            case OP_LT: result = l < r; out_type = TYPE_BOOL; break;
            case OP_GE: result = l >= r; out_type = TYPE_BOOL; break;
            case OP_LE: result = l <= r; out_type = TYPE_BOOL; break;
            case OP_EQ: result = l == r; out_type = TYPE_BOOL; break;
            case OP_NE: result = l != r; out_type = TYPE_BOOL; break;
            default: is_foldable = 0; break;
        }
        if (is_foldable) {
            Expr *folded = alloc_expr(line, EXPR_LITERAL);
            folded->as.literal.type = out_type;
            folded->as.literal.number = result;
            ast_free_expr(left);
            ast_free_expr(right);
            return folded;
        }
    }

    Expr *e = alloc_expr(line, EXPR_BINARY);
    e->as.binary.op = op;
    e->as.binary.left = left;
    e->as.binary.right = right;
    return e;
}

Expr *ast_make_unary(int line, Operator op, Expr *expr) {
    if (op == OP_NEG && is_literal_expr(expr)) {
        Expr *folded = alloc_expr(line, EXPR_LITERAL);
        folded->as.literal.type = expr->as.literal.type;
        folded->as.literal.number = -expr->as.literal.number;
        ast_free_expr(expr);
        return folded;
    }

    Expr *e = alloc_expr(line, EXPR_UNARY);
    e->as.unary.op = op;
    e->as.unary.expr = expr;
    return e;
}

Expr *ast_make_call(int line, const char *name, ExprList args) {
    Expr *e = alloc_expr(line, EXPR_CALL);
    e->as.call.name = xstrdup(name);
    e->as.call.args = args;
    return e;
}

Stmt *ast_make_block(int line, StmtList items) {
    Stmt *s = alloc_stmt(line, STMT_BLOCK);
    s->as.block = items;
    return s;
}

Stmt *ast_make_decl(int line, ValueType type, const char *name, Expr *init) {
    Stmt *s = alloc_stmt(line, STMT_DECL);
    s->as.decl.type = type;
    s->as.decl.name = xstrdup(name);
    s->as.decl.init = init;
    return s;
}

Stmt *ast_make_assign(int line, const char *name, Expr *value) {
    Stmt *s = alloc_stmt(line, STMT_ASSIGN);
    s->as.assign.name = xstrdup(name);
    s->as.assign.value = value;
    return s;
}

Stmt *ast_make_input(int line, const char *name) {
    Stmt *s = alloc_stmt(line, STMT_INPUT);
    s->as.input.name = xstrdup(name);
    return s;
}

Stmt *ast_make_output(int line, Expr *value) {
    Stmt *s = alloc_stmt(line, STMT_OUTPUT);
    s->as.output.value = value;
    return s;
}

Stmt *ast_make_if(int line, Expr *cond, Stmt *then_branch, Stmt *else_branch) {
    Stmt *s = alloc_stmt(line, STMT_IF);
    s->as.if_stmt.cond = cond;
    s->as.if_stmt.then_branch = then_branch;
    s->as.if_stmt.else_branch = else_branch;
    return s;
}

Stmt *ast_make_while(int line, Expr *cond, Stmt *body) {
    Stmt *s = alloc_stmt(line, STMT_WHILE);
    s->as.while_stmt.cond = cond;
    s->as.while_stmt.body = body;
    return s;
}

Stmt *ast_make_for(int line, Stmt *init, Expr *cond, Stmt *update, Stmt *body) {
    Stmt *s = alloc_stmt(line, STMT_FOR);
    s->as.for_stmt.init = init;
    s->as.for_stmt.cond = cond;
    s->as.for_stmt.update = update;
    s->as.for_stmt.body = body;
    return s;
}

Stmt *ast_make_func_def(int line, const char *name, ValueType return_type, Stmt *body) {
    Stmt *s = alloc_stmt(line, STMT_FUNC_DEF);
    s->as.func_def.name = xstrdup(name);
    s->as.func_def.return_type = return_type;
    s->as.func_def.body = body;
    return s;
}

Stmt *ast_make_return(int line, Expr *value) {
    Stmt *s = alloc_stmt(line, STMT_RETURN);
    s->as.return_stmt.value = value;
    return s;
}

Stmt *ast_make_expr_stmt(int line, Expr *expr) {
    Stmt *s = alloc_stmt(line, STMT_EXPR);
    s->as.expr_stmt.expr = expr;
    return s;
}

void ast_free_expr(Expr *expr) {
    int i;
    if (!expr) {
        return;
    }

    switch (expr->kind) {
        case EXPR_VARIABLE:
            free(expr->as.var_name);
            break;
        case EXPR_BINARY:
            ast_free_expr(expr->as.binary.left);
            ast_free_expr(expr->as.binary.right);
            break;
        case EXPR_UNARY:
            ast_free_expr(expr->as.unary.expr);
            break;
        case EXPR_CALL:
            free(expr->as.call.name);
            for (i = 0; i < expr->as.call.args.count; ++i) {
                ast_free_expr(expr->as.call.args.items[i]);
            }
            free(expr->as.call.args.items);
            break;
        case EXPR_LITERAL:
        default:
            break;
    }

    free(expr);
}

void ast_free_stmt(Stmt *stmt) {
    int i;
    if (!stmt) {
        return;
    }

    switch (stmt->kind) {
        case STMT_BLOCK:
            for (i = 0; i < stmt->as.block.count; ++i) {
                ast_free_stmt(stmt->as.block.items[i]);
            }
            free(stmt->as.block.items);
            break;
        case STMT_DECL:
            free(stmt->as.decl.name);
            ast_free_expr(stmt->as.decl.init);
            break;
        case STMT_ASSIGN:
            free(stmt->as.assign.name);
            ast_free_expr(stmt->as.assign.value);
            break;
        case STMT_INPUT:
            free(stmt->as.input.name);
            break;
        case STMT_OUTPUT:
            ast_free_expr(stmt->as.output.value);
            break;
        case STMT_IF:
            ast_free_expr(stmt->as.if_stmt.cond);
            ast_free_stmt(stmt->as.if_stmt.then_branch);
            ast_free_stmt(stmt->as.if_stmt.else_branch);
            break;
        case STMT_WHILE:
            ast_free_expr(stmt->as.while_stmt.cond);
            ast_free_stmt(stmt->as.while_stmt.body);
            break;
        case STMT_FOR:
            ast_free_stmt(stmt->as.for_stmt.init);
            ast_free_expr(stmt->as.for_stmt.cond);
            ast_free_stmt(stmt->as.for_stmt.update);
            ast_free_stmt(stmt->as.for_stmt.body);
            break;
        case STMT_FUNC_DEF:
            free(stmt->as.func_def.name);
            ast_free_stmt(stmt->as.func_def.body);
            break;
        case STMT_RETURN:
            ast_free_expr(stmt->as.return_stmt.value);
            break;
        case STMT_EXPR:
            ast_free_expr(stmt->as.expr_stmt.expr);
            break;
    }

    free(stmt);
}
