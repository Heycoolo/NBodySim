#include "../include/body.h"
#include <stdlib.h>



void initialize_body_state_array(Body *b) {


    State *arr = malloc(b->n_steps * sizeof(State));
    if (arr == NULL) {
        exit(1);
    }
    b->sol_arr = arr;
    return;
}