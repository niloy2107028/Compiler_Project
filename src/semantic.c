#include "semantic.h"

#include "symbol_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    ValueType type;
    double number;
    int valid;
} Value;

typedef struct {
    char *name;
    ValueType return_type;
    Stmt *body;
} Function;

typedef struct {
    SymbolTable symbols;
    Function *functions;
    int function_count;
    int function_cap;
    int error_count;
    int returning;
    Value return_value;
} ExecContext;

static void report_error(ExecContext *ctx, int line, const char *msg) {
    fprintf(stderr, "Semantic error (line %d): %s\n", line, msg);
    ctx->error_count++;
}

static Value make_value(ValueType type, double number, int valid) {
    Value v;
    v.type = type;
    v.number = number;
    v.valid = valid;
    return v;
}

static Value invalid_value(void) {
    return make_value(TYPE_ERROR, 0.0, 0);
}

static int as_bool(Value value) {
    return value.number != 0.0;
}

static int can_assign(ValueType target, ValueType source) {
    if (target == source) {
        return 1;
    }
    if (target == TYPE_FLOAT && source == TYPE_INT) {
        return 1;
    }
    if (target == TYPE_INT && source == TYPE_BOOL) {
        return 1;
    }
    if (target == TYPE_FLOAT && source == TYPE_BOOL) {
        return 1;
    }
    return 0;
}

static double convert_number(ValueType target, Value value) {
    if (target == TYPE_INT || target == TYPE_BOOL) {
        return (long)value.number;
    }
    return value.number;
}

static void ensure_function_capacity(ExecContext *ctx) {
    if (ctx->function_count == ctx->function_cap) {
        int new_cap = ctx->function_cap == 0 ? 8 : ctx->function_cap * 2;
        ctx->functions = (Function *)realloc(ctx->functions, sizeof(Function) * (size_t)new_cap);
        ctx->function_cap = new_cap;
    }
}

static char *xstrdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = (char *)calloc(len, 1);
    if (!copy) {
        exit(1);
    }
    memcpy(copy, s, len);
    return copy;
}

static Function *find_function(ExecContext *ctx, const char *name) {
    int i;
    for (i = 0; i < ctx->function_count; ++i) {
        if (strcmp(ctx->functions[i].name, name) == 0) {
            return &ctx->functions[i];
        }
    }
    return NULL;
}

static Value eval_expr(ExecContext *ctx, Expr *expr);
static void exec_stmt(ExecContext *ctx, Stmt *stmt);

static Value eval_binary(ExecContext *ctx, Expr *expr) {
    Value left = eval_expr(ctx, expr->as.binary.left);
    Value right = eval_expr(ctx, expr->as.binary.right);
    Operator op = expr->as.binary.op;
    ValueType num_type = (left.type == TYPE_FLOAT || right.type == TYPE_FLOAT) ? TYPE_FLOAT : TYPE_INT;

    if (!left.valid || !right.valid) {
        return invalid_value();
    }

    switch (op) {
        case OP_ADD:
            return make_value(num_type, left.number + right.number, 1);
        case OP_SUB:
            return make_value(num_type, left.number - right.number, 1);
        case OP_MUL:
            return make_value(num_type, left.number * right.number, 1);
        case OP_DIV:
            if (right.number == 0.0) {
                report_error(ctx, expr->line, "Division by zero");
                return invalid_value();
            }
            return make_value(TYPE_FLOAT, left.number / right.number, 1);
        case OP_GT:
            return make_value(TYPE_BOOL, left.number > right.number, 1);
        case OP_LT:
            return make_value(TYPE_BOOL, left.number < right.number, 1);
        case OP_GE:
            return make_value(TYPE_BOOL, left.number >= right.number, 1);
        case OP_LE:
            return make_value(TYPE_BOOL, left.number <= right.number, 1);
        case OP_EQ:
            return make_value(TYPE_BOOL, left.number == right.number, 1);
        case OP_NE:
            return make_value(TYPE_BOOL, left.number != right.number, 1);
        default:
            return invalid_value();
    }
}

static Value eval_expr(ExecContext *ctx, Expr *expr) {
    Symbol *sym;
    Function *fn;
    int saved_returning;
    Value saved_return;

    if (!expr) {
        return invalid_value();
    }

    switch (expr->kind) {
        case EXPR_LITERAL:
            return make_value(expr->as.literal.type, expr->as.literal.number, 1);
        case EXPR_VARIABLE:
            sym = symtab_lookup(&ctx->symbols, expr->as.var_name);
            if (!sym) {
                report_error(ctx, expr->line, "Undeclared variable used");
                return invalid_value();
            }
            if (!sym->initialized) {
                report_error(ctx, expr->line, "Variable used before assignment");
                return invalid_value();
            }
            return make_value(sym->type, sym->value, 1);
        case EXPR_UNARY: {
            Value inner = eval_expr(ctx, expr->as.unary.expr);
            if (!inner.valid) {
                return invalid_value();
            }
            if (expr->as.unary.op == OP_NEG) {
                return make_value(inner.type, -inner.number, 1);
            }
            return invalid_value();
        }
        case EXPR_BINARY:
            return eval_binary(ctx, expr);
        case EXPR_CALL:
            fn = find_function(ctx, expr->as.call.name);
            if (!fn) {
                report_error(ctx, expr->line, "Function not declared");
                return invalid_value();
            }
            if (expr->as.call.args.count != 0) {
                report_error(ctx, expr->line, "Only zero-argument User Defined functions are supported");
                return invalid_value();
            }

            saved_returning = ctx->returning;
            saved_return = ctx->return_value;
            ctx->returning = 0;
            ctx->return_value = invalid_value();
            exec_stmt(ctx, fn->body);
            if (!ctx->returning) {
                report_error(ctx, expr->line, "Function did not return a value");
                ctx->returning = saved_returning;
                ctx->return_value = saved_return;
                return invalid_value();
            }
            if (!can_assign(fn->return_type, ctx->return_value.type)) {
                report_error(ctx, expr->line, "Function return type mismatch");
                ctx->returning = saved_returning;
                ctx->return_value = saved_return;
                return invalid_value();
            }
            {
                Value out = make_value(fn->return_type, convert_number(fn->return_type, ctx->return_value), 1);
                ctx->returning = saved_returning;
                ctx->return_value = saved_return;
                return out;
            }
        default:
            return invalid_value();
    }
}

static void exec_block(ExecContext *ctx, StmtList block, int new_scope) {
    int i;
    if (new_scope) {
        symtab_push_scope(&ctx->symbols);
    }
    for (i = 0; i < block.count; ++i) {
        exec_stmt(ctx, block.items[i]);
        if (ctx->returning) {
            break;
        }
    }
    if (new_scope) {
        symtab_pop_scope(&ctx->symbols);
    }
}

static void exec_stmt(ExecContext *ctx, Stmt *stmt) {
    Symbol *sym;
    Value v;

    if (!stmt || ctx->error_count > 0) {
        return;
    }

    switch (stmt->kind) {
        case STMT_BLOCK:
            exec_block(ctx, stmt->as.block, 1);
            break;
        case STMT_DECL:
            if (!symtab_declare(&ctx->symbols, stmt->as.decl.name, stmt->as.decl.type)) {
                report_error(ctx, stmt->line, "Duplicate declaration in the same scope");
                break;
            }
            if (stmt->as.decl.init) {
                sym = symtab_lookup(&ctx->symbols, stmt->as.decl.name);
                v = eval_expr(ctx, stmt->as.decl.init);
                if (!v.valid) {
                    break;
                }
                if (!can_assign(sym->type, v.type)) {
                    report_error(ctx, stmt->line, "Type mismatch in declaration assignment");
                    break;
                }
                sym->value = convert_number(sym->type, v);
                sym->initialized = 1;
            }
            break;
        case STMT_ASSIGN:
            sym = symtab_lookup(&ctx->symbols, stmt->as.assign.name);
            if (!sym) {
                report_error(ctx, stmt->line, "Assignment to undeclared variable");
                break;
            }
            v = eval_expr(ctx, stmt->as.assign.value);
            if (!v.valid) {
                break;
            }
            if (!can_assign(sym->type, v.type)) {
                report_error(ctx, stmt->line, "Implicit conversion not allowed (float to int)");
                break;
            }
            sym->value = convert_number(sym->type, v);
            sym->initialized = 1;
            break;
        case STMT_INPUT:
            sym = symtab_lookup(&ctx->symbols, stmt->as.input.name);
            if (!sym) {
                report_error(ctx, stmt->line, "Input target variable is undeclared");
                break;
            }
            {
                double in_value = 0.0;
                if (scanf("%lf", &in_value) != 1) {
                    report_error(ctx, stmt->line, "Failed to read input");
                    break;
                }
                sym->value = convert_number(sym->type, make_value(TYPE_FLOAT, in_value, 1));
                sym->initialized = 1;
            }
            break;
        case STMT_OUTPUT:
            v = eval_expr(ctx, stmt->as.output.value);
            if (!v.valid) {
                break;
            }
            if (v.type == TYPE_INT || v.type == TYPE_BOOL) {
                printf("%ld\n", (long)v.number);
            } else {
                printf("%.6f\n", v.number);
            }
            break;
        case STMT_IF:
            v = eval_expr(ctx, stmt->as.if_stmt.cond);
            if (!v.valid) {
                break;
            }
            if (as_bool(v)) {
                exec_stmt(ctx, stmt->as.if_stmt.then_branch);
            } else {
                exec_stmt(ctx, stmt->as.if_stmt.else_branch);
            }
            break;
        case STMT_WHILE:
            while (1) {
                v = eval_expr(ctx, stmt->as.while_stmt.cond);
                if (!v.valid || !as_bool(v)) {
                    break;
                }
                exec_stmt(ctx, stmt->as.while_stmt.body);
                if (ctx->returning) {
                    break;
                }
            }
            break;
        case STMT_FOR:
            exec_stmt(ctx, stmt->as.for_stmt.init);
            while (1) {
                v = eval_expr(ctx, stmt->as.for_stmt.cond);
                if (!v.valid || !as_bool(v)) {
                    break;
                }
                exec_stmt(ctx, stmt->as.for_stmt.body);
                if (ctx->returning) {
                    break;
                }
                exec_stmt(ctx, stmt->as.for_stmt.update);
            }
            break;
        case STMT_FUNC_DEF:
            break;
        case STMT_RETURN:
            if (!stmt->as.return_stmt.value) {
                report_error(ctx, stmt->line, "Return statement requires a value");
                break;
            }
            v = eval_expr(ctx, stmt->as.return_stmt.value);
            if (!v.valid) {
                break;
            }
            ctx->returning = 1;
            ctx->return_value = v;
            break;
        case STMT_EXPR:
            (void)eval_expr(ctx, stmt->as.expr_stmt.expr);
            break;
    }
}

static int collect_functions(ExecContext *ctx, Stmt *program) {
    int i;
    if (!program || program->kind != STMT_BLOCK) {
        return 0;
    }
    for (i = 0; i < program->as.block.count; ++i) {
        Stmt *stmt = program->as.block.items[i];
        if (stmt->kind == STMT_FUNC_DEF) {
            if (find_function(ctx, stmt->as.func_def.name)) {
                report_error(ctx, stmt->line, "Duplicate function declaration");
                continue;
            }
            ensure_function_capacity(ctx);
            ctx->functions[ctx->function_count].name = xstrdup(stmt->as.func_def.name);
            ctx->functions[ctx->function_count].return_type = stmt->as.func_def.return_type;
            ctx->functions[ctx->function_count].body = stmt->as.func_def.body;
            ctx->function_count++;
        }
    }
    return ctx->error_count == 0;
}

RunResult execute_program(Stmt *program) {
    ExecContext ctx;
    RunResult result;
    int i;

    ctx.functions = NULL;
    ctx.function_count = 0;
    ctx.function_cap = 0;
    ctx.error_count = 0;
    ctx.returning = 0;
    ctx.return_value = invalid_value();

    symtab_init(&ctx.symbols);

    if (collect_functions(&ctx, program)) {
        if (program && program->kind == STMT_BLOCK) {
            exec_block(&ctx, program->as.block, 0);
        }
    }

    result.success = ctx.error_count == 0;
    result.error_count = ctx.error_count;

    for (i = 0; i < ctx.function_count; ++i) {
        free(ctx.functions[i].name);
    }
    free(ctx.functions);
    symtab_free(&ctx.symbols);

    return result;
}
