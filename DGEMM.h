#ifndef DGEMM_H
#define DGEMM_H

#include <stdlib.h>

void dgemm(size_t, double*, double*, double*);
void dgemm_optimized(size_t, double*, double*, double*);
void aloca_matrizes(const size_t, double* , double*, double*);

#endif