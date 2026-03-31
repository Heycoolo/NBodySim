#ifndef SYSTEM_H
#define SYSTEM_H

#include "../include/body.h"

struct system {
    size_t n_bodies;
    size_t n_dim;
    Body *bodies;
};
typedef struct system System;


#endif