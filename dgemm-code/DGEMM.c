#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
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

void dgemm_AVX(const size_t n, double *A, double *B, double *C){
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

void dgemm_AVX_unroll(const size_t n, double *A, double *B, double *C){
    for(size_t i = 0; i < n; i+=UNROLL*4){
        for(size_t j = 0; j < n; j++){
            __m256d c[UNROLL];
            
            for(size_t x = 0; x < UNROLL; x++) c[x] = _mm256_load_pd(C + i + x*UNROLL + j*n);

            for(size_t k = 0; k < n; k++){
                __m256d b = _mm256_broadcast_sd(B + k + j*n);
                for(size_t x = 0; x < UNROLL; x++){
                    c[x] = _mm256_add_pd(c[x], _mm256_mul_pd(_mm256_load_pd(A + i + x*UNROLL + k*n), b));
                    _mm256_store_pd(C + i + x*UNROLL + j*n, c[x]);
                }
            }
            //for(size_t x = 0; i < UNROLL; x++) _mm256_store_pd(C + i + x*UNROLL + j*n, c[x]); // Ofast e O3 simplesmente ignoram esse loop
        }
    }
}

void do_block(const size_t n, const size_t si, const size_t sj, const size_t sk, double *A, double *B, double *C){
    for(size_t i = si; i < si+BLOCKSIZE; i+=UNROLL*4){
        for(size_t j = sj; j < sj+BLOCKSIZE; j++){
            __m256d c[UNROLL];
            for(size_t x = 0; x < UNROLL; x++) c[x] = _mm256_load_pd(C + i + x*UNROLL + j*n);
            for(size_t k = sk; k < sk+BLOCKSIZE; k++){
                __m256d b = _mm256_broadcast_sd(B + k + j*n);
                for(size_t x = 0; x < UNROLL; x++){
                    c[x] = _mm256_add_pd(c[x], _mm256_mul_pd(_mm256_load_pd(A + i + x*UNROLL + k*n), b));
                }

                for(size_t x = 0; x < UNROLL; x++) _mm256_store_pd(C + i + x*UNROLL + j*n, c[x]);
            }
        }
    }
}

void dgemm_AVX_BLOCK(const size_t n, double *A, double *B, double *C){
    for(size_t sj = 0; sj < n; sj += BLOCKSIZE){
        for(size_t si = 0; si < n; si += BLOCKSIZE){
            for(size_t sk = 0; sk < n; sk += BLOCKSIZE){
                do_block(n, si, sj, sk, A, B, C);
            }
        }
    }
}

void aloca_matrizes(const size_t size, double **A, double **B, double **C){
    if(A != NULL) *A = (double *) aligned_alloc(32, size * sizeof(double));
    if(B != NULL) *B = (double *) aligned_alloc(32, size * sizeof(double));
    if(C != NULL) *C = (double *) aligned_alloc(32, size * sizeof(double));
}

void inicializa_matrizes(const size_t size, double *A, double *B, double *C){
    for(size_t i = 0; i < size; i++){
        for(size_t j = 0; j < size; j++){
                // atribui valores aleatórios 
                if(A != NULL) A[i + j*size] = rand_double();
                if(B != NULL) B[i + j*size] = rand_double();
                if(C != NULL) C[i + j*size] = 0.0;
        }
    }
}

double rand_double(){
    return (MIN_DOUBLE + (rand()*(MAX_DOUBLE - MIN_DOUBLE)/RAND_MAX));
}

double compara_matrizes(const size_t size, const double *C1, const double *C2, const double *C3, double *err_AVX, double *err_UNROLL){
    double erro_medio = 0, erro;

    *err_AVX = 0;
    *err_UNROLL = 0;

    for(size_t i = 0; i < size; i++){
        for(size_t j = 0; j < size; j++){
            //printf("C1[%zu][%zu] = %.2f\n", i, j, C1[i + j*size]);
            //printf("C2[%zu][%zu] = %.2f\n", i, j, C2[i + j*size]);
            //printf("C3[%zu][%zu] = %.2f\n", i, j, C3[i + j*size]);

            erro = C1[i + j*size] - C2[i + j*size];
            *err_AVX += erro > 0 ? erro : -1*erro;
            erro = C1[i + j*size] - C3[i + j*size];
            *err_UNROLL += erro > 0 ? erro : -1*erro;
        }
    }

    erro_medio = (*err_AVX + *err_UNROLL)/2;
    
    return erro_medio;
}