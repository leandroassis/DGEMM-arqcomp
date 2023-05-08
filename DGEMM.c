#include <stdlib.h>
#include "DGEMM.h"

void dgemm(const size_t n, const double *A, const double *B, double *C){

    for(size_t i = 0; i < n; i++){
        for(size_t j = 0; j < n; j++){
            double cij = C[i+j*n];
            for(size_t k = 0; k < n; k++){
                cij += A[i + k*n] * B[k + j*n];
                C[i + j*n] = cij;
            }
        }
    }
}

void aloca_matrizes(const size_t size, double **A, double **B, double **C){

   // aloca memoria para as matrizes
    A = (double **) malloc(size*sizeof(double*));
    B = (double **) malloc(size*sizeof(double*));
    C = (double **) malloc(size*sizeof(double*));

    for(size_t i = 0; i < size; i++){
        A[i] = (double *) malloc(size*sizeof(double));
        B[i] = (double *) malloc(size*sizeof(double));
        C[i] = (double *) malloc(size*sizeof(double));
    }
}