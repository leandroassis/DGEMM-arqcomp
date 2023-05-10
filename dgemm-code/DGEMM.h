#ifndef DGEMM_H
#define DGEMM_H

#define SEED                 0x12345678
#define NOME_ARQUIVO         "resultadosO3.txt"

// tamanho da matriz
#define NUM_ITERACOES       10
#define TAMANHO_MIN_MATRIZES 64 // 64 linhas e 64 colunas = 4096 células aprox 32kB
#define TAMANHO_MAX_MATRIZES 1024  // 1024 linhas e 1024 colunas = 1048576 células aprox 8GB
#define NUM_RESULTADOS_DESEJADOS 10
#define STEP_MATRIZES        (TAMANHO_MAX_MATRIZES-TAMANHO_MIN_MATRIZES)/NUM_RESULTADOS_DESEJADOS // deve ser multiplo de 32 assim como os tamanhos min e max das matrizes

// valores inseridos na matriz
#define MIN_DOUBLE           -100000.0
#define MAX_DOUBLE            100000.0

#include <stdlib.h>

void dgemm(const size_t, const double*, const double*, double*);
//void dgemm_optimized(const size_t, double*, double*, double*);

void aloca_matrizes(const size_t size, double **, double **, double **);
void inicializa_matrizes(const size_t size, double *, double *, double *);
double rand_double();

#endif