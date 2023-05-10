#include <stdio.h>
#include <stdlib.h>
//#include <x86intrin.h>
#include "DGEMM.h"

void dgemm(const size_t n, const double *A, const double *B, double *C){
// código retirado do livro
    for(size_t i = 0; i < n; i++){
        for(size_t j = 0; j < n; j++){
            double cij = C[i+j*n];
            for(size_t k = 0; k < n; k++){
                cij += A[i + k*n] * B[k + j*n];
            }
            C[i + j*n] = cij;
        }
    }
}

/*
void dgemm_optimized(const size_t n, const double *A, const double *B, double *C){
// código retirado do livro
    for(size_t i = 0; i < n; i+=4){
        for(size_t j = 0; j < n; j++){
            __m256d c0 = _mm256_load_pd(C + i + j*n);
            for(size_t k = 0; k < n; k++){
                c0 = _mm256_add_pd(c0, _mm256_mul_pd(_mm256_load_pd(A + i + k*n), _mm256_broadcast_sd(B + k + j*n)));
                _mm256_store_pd(C + i + j*n, c0);
            }
        }
    }
}
*/

void aloca_matrizes(const size_t size, double **A, double **B, double **C){
    *A = (double *) malloc(size * sizeof(double));
    *B = (double *) malloc(size * sizeof(double));
    *C = (double *) malloc(size * sizeof(double));
}

void inicializa_matrizes(const size_t size, double *A, double *B, double *C){

    for(size_t i = 0; i < size; i++){
        for(size_t j = 0; j < size; j++){
                // atribui valores aleatórios 
                A[i + j*size] = rand_double();
                B[i + j*size] = rand_double();
                C[i + j*size] = 0.0;
        }
    }

}

double rand_double(){
    return (MIN_DOUBLE + (rand()*(MAX_DOUBLE - MIN_DOUBLE)/RAND_MAX));
}