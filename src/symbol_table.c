#include "symbol_table.h"

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

void symtab_init(SymbolTable *table) {
    table->items = NULL;
    table->count = 0;
    table->cap = 0;
    table->current_scope = 0;
}

void symtab_free(SymbolTable *table) {
    int i;
    for (i = 0; i < table->count; ++i) {
        free(table->items[i].name);
    }
    free(table->items);
    table->items = NULL;
    table->count = 0;
    table->cap = 0;
}

void symtab_push_scope(SymbolTable *table) {
    table->current_scope++;
}

void symtab_pop_scope(SymbolTable *table) {
    int i = table->count - 1;
    while (i >= 0) {
        if (table->items[i].scope == table->current_scope) {
            free(table->items[i].name);
            table->items[i] = table->items[table->count - 1];
            table->count--;
            i = table->count - 1;
            continue;
        }
        i--;
    }
    if (table->current_scope > 0) {
        table->current_scope--;
    }
}

static void ensure_capacity(SymbolTable *table) {
    if (table->count == table->cap) {
        int new_cap = table->cap == 0 ? 16 : table->cap * 2;
        table->items = (Symbol *)realloc(table->items, sizeof(Symbol) * (size_t)new_cap);
        table->cap = new_cap;
    }
}

int symtab_declare(SymbolTable *table, const char *name, ValueType type) {
    if (symtab_lookup_current_scope(table, name)) {
        return 0;
    }
    ensure_capacity(table);
    table->items[table->count].name = xstrdup(name);
    table->items[table->count].type = type;
    table->items[table->count].value = 0.0;
    table->items[table->count].initialized = 0;
    table->items[table->count].scope = table->current_scope;
    table->count++;
    return 1;
}

Symbol *symtab_lookup(SymbolTable *table, const char *name) {
    int i;
    int best_scope = -1;
    Symbol *best = NULL;
    for (i = 0; i < table->count; ++i) {
        if (strcmp(table->items[i].name, name) == 0 && table->items[i].scope >= best_scope) {
            best_scope = table->items[i].scope;
            best = &table->items[i];
        }
    }
    return best;
}

Symbol *symtab_lookup_current_scope(SymbolTable *table, const char *name) {
    int i;
    for (i = 0; i < table->count; ++i) {
        if (table->items[i].scope == table->current_scope && strcmp(table->items[i].name, name) == 0) {
            return &table->items[i];
        }
    }
    return NULL;
}
