#include "../include/runge_kutta.h"


double* RK4(double *state,
            double step_size,
            size_t arr_len,
            double *(*eval_func)(double *state, size_t arr_len)) {

    double *k1 = eval_func(state, arr_len);


    

    
    return;
}