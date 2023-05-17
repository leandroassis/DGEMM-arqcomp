#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "DGEMM.h"

int main(void){

    double *matrixA, *matrixB, *matrixC;
    struct timespec t_ini = {0, 0}, t_fim = {0, 0};
    float t_exe;
    FILE *fp;
    
    // seeda o gerador de números aleatórios
    srand(SEED);

    // abre o arquivo para escrita
    fp = fopen(NOME_ARQUIVO, "w");
    fprintf(fp, "Tamanho da matriz;Tempo DGEMM normal;Tempo DGEMM AVX; Tempo DGEMM AVX + UNROLL\n");
    
    for(int i = 0; i < NUM_ITERACOES; i++){

        for(size_t size = TAMANHO_MIN_MATRIZES; size <= TAMANHO_MAX_MATRIZES; size+=STEP_MATRIZES){

            // aloca memoria para as matrizes A, B e C se beneficiando da localidade espacial
            // pois os acessos à memória são agrupados
            aloca_matrizes(size*size, &matrixA, &matrixB, &matrixC);

            // inicializa as matrizes com valores "aleatórios"
            inicializa_matrizes(size, matrixA, matrixB, matrixC);

            fprintf(fp, "%zu;", size*size);

            // roda o algoritmo DGEMM normal
            clock_gettime(CLOCK_MONOTONIC, &t_ini); // tempo de inicio
            dgemm(size, matrixA, matrixB, matrixC);
            clock_gettime(CLOCK_MONOTONIC, &t_fim); // tempo final
            t_exe = (t_fim.tv_sec - t_ini.tv_sec) + (t_fim.tv_nsec - t_ini.tv_nsec) / 1e9; // tempo de execução

            printf("Tempo de execução do algoritmo DGEMM normal com matriz de tamanho %.2f MB: %.6f s.\n", (float) (size*size/(1024*1024)), t_exe);
            fprintf(fp, "%.6f;", t_exe);

            // roda o algoritmo DGEMM otimizado com AVX
            clock_gettime(CLOCK_MONOTONIC, &t_ini);
            dgemm_AVX(size, matrixA, matrixB, matrixC);
            clock_gettime(CLOCK_MONOTONIC, &t_fim);
            t_exe = (t_fim.tv_sec - t_ini.tv_sec) + (t_fim.tv_nsec - t_ini.tv_nsec) / 1e9; // tempo de execução

            printf("Tempo de execução do algoritmo DGEMM melhorado com matriz de tamanho %.2f MB: %.6f s.\n", (float) (size*size/(1024*1024)), t_exe);
            fprintf(fp, "%.6f;", t_exe);

            // roda o algoritmo DGEMM otimizado com AVX e UNROLL
            clock_gettime(CLOCK_MONOTONIC, &t_ini);
            dgemm_AVX_unroll((int) size, matrixA, matrixB, matrixC);
            clock_gettime(CLOCK_MONOTONIC, &t_fim);
            t_exe = (t_fim.tv_sec - t_ini.tv_sec) + (t_fim.tv_nsec - t_ini.tv_nsec) / 1e9; // tempo de execução

            printf("Tempo de execução do algoritmo DGEMM melhorado com matriz de tamanho %.2f MB: %.6f s.\n", (float) (size*size/(1024*1024)), t_exe);
            fprintf(fp, "%.6f\n", t_exe);

            free(matrixA);
            free(matrixB);
            free(matrixC);
            printf("\n");
        }
    }

    fclose(fp);

    return 0;
}