#include <stdlib.h>
#include <pthread.h>
#include "../structures.h"
#include "../execute.h"

void * parallel_routine(void * bundle_ptr){
    struct Execution_bundle * bundle = (struct Execution_bundle *) bundle_ptr;
    execute(bundle->ast, bundle->defined, bundle->let_map);
    return 0;
}

void parallel_execution(struct Tree * ast, struct Tree_map * defined, struct Map * let_map){
    int num_children = ast->size;
    pthread_t tid[num_children];
    struct Execution_bundle * bundle[num_children];
    for(int i = 0; i < num_children; i++){
        bundle[i] = malloc(sizeof(struct Execution_bundle));
        bundle[i]->ast=ast->children[i];
        bundle[i]->defined=defined;
        bundle[i]->let_map=let_map;
        pthread_create(&tid[i], NULL, &parallel_routine, bundle[i]);
    }
    for (int j = 0; j < num_children; j++){
        pthread_join(tid[j], NULL);
        free(bundle[j]);
    }
}
