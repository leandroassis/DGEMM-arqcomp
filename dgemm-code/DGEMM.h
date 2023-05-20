#ifndef DGEMM_H
#define DGEMM_H

#define SEED                 0x12345678

// tamanho da matriz
#define NUM_ITERACOES       3
#define TAMANHO_MIN_MATRIZES 4096 // 4096 linhas e 4096 colunas = 16.777.216 células aprox 16MB
#define TAMANHO_MAX_MATRIZES 8192 // 8192 linhas e 8192 colunas = 67.108.864 células aprox 64MB
#define NUM_RESULTADOS_DESEJADOS 4
#define STEP_MATRIZES        (TAMANHO_MAX_MATRIZES-TAMANHO_MIN_MATRIZES)/NUM_RESULTADOS_DESEJADOS // deve ser multiplo de 32 assim como os tamanhos min e max das matrizes

// valores inseridos na matriz
#define MIN_DOUBLE           -100000.0
#define MAX_DOUBLE            100000.0

#define UNROLL 4

#include <stdlib.h>

void dgemm(const size_t, const double*, const double*, double*);
void dgemm_AVX(const size_t, double*, double*, double*);
void dgemm_AVX_unroll(const int, double*, double*, double*);

void aloca_matrizes(const size_t size, double **, double **, double **);
void inicializa_matrizes(const size_t size, double *, double *, double *);
double rand_double();

#endif
