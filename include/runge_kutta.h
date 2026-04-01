#ifndef RUNGE_KUTTA_H
#define RUNGE_KUTTA_H

#include <stddef.h>
#include "../include/system.h"

void RK4(System * const sys,
            void (*eval_func)(State *k, State *curr_state, System *sys));


#endif