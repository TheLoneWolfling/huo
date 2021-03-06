#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "tokenizer.h"
#include "parser.h"
#include "structures.h"
#include "execute.h"
#include "store_defs.h"
#include "base_util.h"

int main(int argc, char const *argv[]) {
    if(argc < 2){
        ERROR("You have run Huo without pointing it to a file.");
    }

    FILE *fp;
    fp = fopen(argv[1], "r");
    if(fp == NULL){
        ERROR("Error opening file: %d (%s)", errno, strerror(errno));
    }

    struct Tokens t = {
        .length = 0,
        .counter = 0
    };
    struct String file = {
        .length = 0,
        .body = NULL
    };

    char c;
    while ((c = fgetc(fp)) != EOF){
        if(c != '\n'){
            RESIZE(file.body, file.length + 1);
            file.body[file.length] = c;
            file.length++;
        }
    }
    fclose(fp);
    
    // Null character at end
    RESIZE(file.body, file.length + 1);
    file.body[file.length] = 0;
    
    assert(string_is_sane(&file));
    

    struct Tokens * tokens = tokenize(file, &t);
    // for(int i = 0; i < tokens->length; i++){
    //     printf("%c", tokens->tokens[i].type);
    // }

    struct Tree root;
    root.type = 'r';
    root.size = 0;

    parse(&root, tokens);
    // this prints the AST for reference
    // printTree(&root);
    // printf("\n");
    struct Tree_map * defined = malloc(sizeof(struct Tree_map));
    defined->size = 0;
    struct Map * let_map = malloc(sizeof(struct Map));
    let_map->size = 0;
    int num_defs = store_defs(&root, defined);
    for(int i = num_defs; i < root.size; i++){
        execute(root.children[i], defined, let_map);
    }
    return 0;
}
