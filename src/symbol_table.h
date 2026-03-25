#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

typedef struct
{
    char *name;
    ValueType type;
    double value;
    int initialized;
    int scope;
} Symbol;

typedef struct
{
    Symbol *items;
    int count;
    int cap;
    int current_scope;
} SymbolTable;

void symtab_init(SymbolTable *table);
void symtab_free(SymbolTable *table);

void symtab_push_scope(SymbolTable *table);
void symtab_pop_scope(SymbolTable *table);

int symtab_declare(SymbolTable *table, const char *name, ValueType type);
Symbol *symtab_lookup(SymbolTable *table, const char *name);
Symbol *symtab_lookup_current_scope(SymbolTable *table, const char *name);

#endif
