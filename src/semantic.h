#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

typedef struct
{
    int success;
    int error_count;
} RunResult;

RunResult execute_program(Stmt *program);

#endif
