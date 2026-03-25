#include "ir.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **lines;
    int count;
    int cap;
    int temp_counter;
    int label_counter;
} TacBuilder;

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

static void append_line(TacBuilder *b, const char *line) {
    if (b->count == b->cap) {
        int new_cap = b->cap == 0 ? 32 : b->cap * 2;
        b->lines = (char **)realloc(b->lines, sizeof(char *) * (size_t)new_cap);
        b->cap = new_cap;
    }
    b->lines[b->count++] = xstrdup(line);
}

static char *new_temp(TacBuilder *b) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "t%d", b->temp_counter++);
    return xstrdup(buffer);
}

static char *new_label(TacBuilder *b) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "L%d", b->label_counter++);
    return xstrdup(buffer);
}

static char *op_text(Operator op) {
    switch (op) {
        case OP_ADD: return "+";
        case OP_SUB: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_GT: return ">";
        case OP_LT: return "<";
        case OP_GE: return ">=";
        case OP_LE: return "<=";
        case OP_EQ: return "==";
        case OP_NE: return "!=";
        default: return "?";
    }
}

static char *emit_expr(TacBuilder *b, Expr *expr);
static void emit_stmt(TacBuilder *b, Stmt *stmt);

static char *emit_literal(Expr *expr) {
    char buffer[64];
    if (expr->as.literal.type == TYPE_INT || expr->as.literal.type == TYPE_BOOL) {
        snprintf(buffer, sizeof(buffer), "%ld", (long)expr->as.literal.number);
    } else {
        snprintf(buffer, sizeof(buffer), "%.6f", expr->as.literal.number);
    }
    return xstrdup(buffer);
}

static char *emit_expr(TacBuilder *b, Expr *expr) {
    char line[256];
    char *left;
    char *right;
    char *tmp;
    int i;

    if (!expr) {
        return xstrdup("0");
    }

    switch (expr->kind) {
        case EXPR_LITERAL:
            return emit_literal(expr);
        case EXPR_VARIABLE:
            return xstrdup(expr->as.var_name);
        case EXPR_UNARY:
            left = emit_expr(b, expr->as.unary.expr);
            tmp = new_temp(b);
            snprintf(line, sizeof(line), "%s = -%s", tmp, left);
            append_line(b, line);
            free(left);
            return tmp;
        case EXPR_BINARY:
            left = emit_expr(b, expr->as.binary.left);
            right = emit_expr(b, expr->as.binary.right);
            tmp = new_temp(b);
            snprintf(line, sizeof(line), "%s = %s %s %s", tmp, left, op_text(expr->as.binary.op), right);
            append_line(b, line);
            free(left);
            free(right);
            return tmp;
        case EXPR_CALL:
            for (i = 0; i < expr->as.call.args.count; ++i) {
                char *arg = emit_expr(b, expr->as.call.args.items[i]);
                snprintf(line, sizeof(line), "param %s", arg);
                append_line(b, line);
                free(arg);
            }
            tmp = new_temp(b);
            snprintf(line, sizeof(line), "%s = call %s, %d", tmp, expr->as.call.name, expr->as.call.args.count);
            append_line(b, line);
            return tmp;
        default:
            return xstrdup("0");
    }
}

static void emit_stmt(TacBuilder *b, Stmt *stmt) {
    int i;
    char line[256];
    char *cond;
    char *value;
    char *label_true;
    char *label_false;
    char *label_start;
    char *label_end;

    if (!stmt) {
        return;
    }

    switch (stmt->kind) {
        case STMT_BLOCK:
            for (i = 0; i < stmt->as.block.count; ++i) {
                emit_stmt(b, stmt->as.block.items[i]);
            }
            break;
        case STMT_DECL:
            snprintf(line, sizeof(line), "decl %s %s", stmt->as.decl.type == TYPE_FLOAT ? "float" : "int", stmt->as.decl.name);
            append_line(b, line);
            if (stmt->as.decl.init) {
                value = emit_expr(b, stmt->as.decl.init);
                snprintf(line, sizeof(line), "%s = %s", stmt->as.decl.name, value);
                append_line(b, line);
                free(value);
            }
            break;
        case STMT_ASSIGN:
            value = emit_expr(b, stmt->as.assign.value);
            snprintf(line, sizeof(line), "%s = %s", stmt->as.assign.name, value);
            append_line(b, line);
            free(value);
            break;
        case STMT_INPUT:
            snprintf(line, sizeof(line), "read %s", stmt->as.input.name);
            append_line(b, line);
            break;
        case STMT_OUTPUT:
            value = emit_expr(b, stmt->as.output.value);
            snprintf(line, sizeof(line), "print %s", value);
            append_line(b, line);
            free(value);
            break;
        case STMT_IF:
            label_true = new_label(b);
            label_false = new_label(b);
            label_end = new_label(b);
            cond = emit_expr(b, stmt->as.if_stmt.cond);
            snprintf(line, sizeof(line), "if %s goto %s", cond, label_true);
            append_line(b, line);
            snprintf(line, sizeof(line), "goto %s", label_false);
            append_line(b, line);
            snprintf(line, sizeof(line), "%s:", label_true);
            append_line(b, line);
            emit_stmt(b, stmt->as.if_stmt.then_branch);
            snprintf(line, sizeof(line), "goto %s", label_end);
            append_line(b, line);
            snprintf(line, sizeof(line), "%s:", label_false);
            append_line(b, line);
            emit_stmt(b, stmt->as.if_stmt.else_branch);
            snprintf(line, sizeof(line), "%s:", label_end);
            append_line(b, line);
            free(cond);
            free(label_true);
            free(label_false);
            free(label_end);
            break;
        case STMT_WHILE:
            label_start = new_label(b);
            label_true = new_label(b);
            label_end = new_label(b);
            snprintf(line, sizeof(line), "%s:", label_start);
            append_line(b, line);
            cond = emit_expr(b, stmt->as.while_stmt.cond);
            snprintf(line, sizeof(line), "if %s goto %s", cond, label_true);
            append_line(b, line);
            snprintf(line, sizeof(line), "goto %s", label_end);
            append_line(b, line);
            snprintf(line, sizeof(line), "%s:", label_true);
            append_line(b, line);
            emit_stmt(b, stmt->as.while_stmt.body);
            snprintf(line, sizeof(line), "goto %s", label_start);
            append_line(b, line);
            snprintf(line, sizeof(line), "%s:", label_end);
            append_line(b, line);
            free(cond);
            free(label_start);
            free(label_true);
            free(label_end);
            break;
        case STMT_FOR:
            emit_stmt(b, stmt->as.for_stmt.init);
            label_start = new_label(b);
            label_true = new_label(b);
            label_end = new_label(b);
            snprintf(line, sizeof(line), "%s:", label_start);
            append_line(b, line);
            cond = emit_expr(b, stmt->as.for_stmt.cond);
            snprintf(line, sizeof(line), "if %s goto %s", cond, label_true);
            append_line(b, line);
            snprintf(line, sizeof(line), "goto %s", label_end);
            append_line(b, line);
            snprintf(line, sizeof(line), "%s:", label_true);
            append_line(b, line);
            emit_stmt(b, stmt->as.for_stmt.body);
            emit_stmt(b, stmt->as.for_stmt.update);
            snprintf(line, sizeof(line), "goto %s", label_start);
            append_line(b, line);
            snprintf(line, sizeof(line), "%s:", label_end);
            append_line(b, line);
            free(cond);
            free(label_start);
            free(label_true);
            free(label_end);
            break;
        case STMT_FUNC_DEF:
            snprintf(line, sizeof(line), "func %s:", stmt->as.func_def.name);
            append_line(b, line);
            emit_stmt(b, stmt->as.func_def.body);
            append_line(b, "endfunc");
            break;
        case STMT_RETURN:
            value = emit_expr(b, stmt->as.return_stmt.value);
            snprintf(line, sizeof(line), "return %s", value);
            append_line(b, line);
            free(value);
            break;
        case STMT_EXPR:
            value = emit_expr(b, stmt->as.expr_stmt.expr);
            free(value);
            break;
    }
}

int generate_tac_file(Stmt *program, const char *out_path) {
    TacBuilder b;
    FILE *fp;
    int i;

    b.lines = NULL;
    b.count = 0;
    b.cap = 0;
    b.temp_counter = 1;
    b.label_counter = 1;

    emit_stmt(&b, program);

    fp = fopen(out_path, "w");
    if (!fp) {
        for (i = 0; i < b.count; ++i) {
            free(b.lines[i]);
        }
        free(b.lines);
        return 0;
    }

    for (i = 0; i < b.count; ++i) {
        fprintf(fp, "%s\n", b.lines[i]);
        free(b.lines[i]);
    }

    free(b.lines);
    fclose(fp);
    return 1;
}
