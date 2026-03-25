#include <stdio.h>
#include <stdlib.h>

#include "../token.h"
#include "ast.h"
#include "ir.h"
#include "semantic.h"

extern FILE *yyin;
#if YYDEBUG
extern int yydebug;
#endif

static void print_usage(const char *prog) {
    fprintf(stderr, "Usage: %s <source_file> [tac_output]\n", prog);
}

int main(int argc, char **argv) {
    FILE *input;
    const char *tac_output;
    RunResult run;

    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    tac_output = (argc >= 3) ? argv[2] : "output.tac";

    input = fopen(argv[1], "r");
    if (!input) {
        perror("Cannot open source file");
        return 1;
    }

    yyin = input;

    {
        const char *trace = getenv("YYDEBUG_TRACE");
#if YYDEBUG
        if (trace && trace[0] == '1') {
            yydebug = 1;
        }
#else
        (void)trace;
#endif
    }

    if (yyparse() != 0 || !g_program_ast) {
        fclose(input);
        fprintf(stderr, "Parsing failed.\n");
        return 1;
    }

    fclose(input);

    run = execute_program(g_program_ast);
    if (!run.success) {
        fprintf(stderr, "Execution failed with %d semantic/runtime error(s).\n", run.error_count);
        ast_free_stmt(g_program_ast);
        return 1;
    }

    if (!generate_tac_file(g_program_ast, tac_output)) {
        fprintf(stderr, "Failed to generate TAC file: %s\n", tac_output);
        ast_free_stmt(g_program_ast);
        return 1;
    }

    printf("Compilation and execution successful. TAC written to %s\n", tac_output);

    ast_free_stmt(g_program_ast);
    return 0;
}
