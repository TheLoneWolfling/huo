#include "../structures.h"
#include "../execute.h"
#include "../base_util.h"

struct Value reduce_ast(struct Tree * ast, struct Tree_map * defined, struct Map * let_map){
    struct Value result = execute(ast->children[0], defined, let_map);

    for(int i = 1; i < ast->size; i++){
        if(ast->children[i]->type == 'f'){
            struct Value newResult = execute(ast->children[i], defined, let_map);
            result = apply_core_function(ast, result, newResult);
        } else {
            result = apply_core_function(ast, result, ast->children[i]->content);
        }
    }
    return result;
}
