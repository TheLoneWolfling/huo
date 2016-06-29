#ifndef _REDUCE_AST_H
#define _REDUCE_AST_H

#include "../structures/structures.h"

struct Value reduce_ast(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes, int max_depth);

#endif