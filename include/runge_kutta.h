#ifndef RUNGE_KUTTA_H
#define RUNGE_KUTTA_H

#include <stddef.h>

double* RK4(double *state,
            double step_size,
            size_t arr_len,
            double *(*eval_func)(double *state, size_t arr_len));


#endif