#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "DGEMM.h"

int main(void){

    double *matrixA, *matrixB, *matrixC;
    struct timespec t_ini = {0, 0}, t_fim = {0, 0};
    float t_exe[2];
    FILE *fp;
    
    // seeda o gerador de números aleatórios
    srand(SEED);

    // abre o arquivo para escrita
    fp = fopen(NOME_ARQUIVO, "w");
    fprintf(fp, "Tamanho da matriz;Tempo DGEMM normal;Tempo DGEMM melhorado\n");
    
    for(int i = 0; i < NUM_ITERACOES; i++){

        for(size_t size = TAMANHO_MIN_MATRIZES; size <= TAMANHO_MAX_MATRIZES; size+=STEP_MATRIZES){

            // aloca memoria para as matrizes A, B e C se beneficiando da localidade espacial
            // pois os acessos à memória são agrupados
            aloca_matrizes(size*size, &matrixA, &matrixB, &matrixC);

            // inicializa as matrizes com valores "aleatórios"
            inicializa_matrizes(size, matrixA, matrixB, matrixC);

            // roda o algoritmo DGEMM normal
            clock_gettime(CLOCK_MONOTONIC, &t_ini); // tempo de inicio
            dgemm(size, matrixA, matrixB, matrixC);
            clock_gettime(CLOCK_MONOTONIC, &t_fim); // tempo final
            t_exe[0] = (t_fim.tv_sec - t_ini.tv_sec) + (t_fim.tv_nsec - t_ini.tv_nsec) / 1e9; // tempo de execução

            printf("Tempo de execução do algoritmo DGEMM normal com matriz de tamanho %.2f MB: %.6f s.\n", (float) (size*size/(1024*1024)), t_exe[0]);

            // roda o algoritmo DGEMM otimizado
            clock_gettime(CLOCK_MONOTONIC, &t_ini);
            dgemm_optimized(size, matrixA, matrixB, matrixC);
            clock_gettime(CLOCK_MONOTONIC, &t_fim);
            t_exe[1] = (t_fim.tv_sec - t_ini.tv_sec) + (t_fim.tv_nsec - t_ini.tv_nsec) / 1e9; // tempo de execução

            printf("Tempo de execução do algoritmo DGEMM melhorado com matriz de tamanho %.2f MB: %.6f s.\n", (float) (size*size/(1024*1024)), t_exe[1]);

            fprintf(fp, "%zu;%.6f;%.6f\n", size*size, t_exe[0], t_exe[1]);

            free(matrixA);
            free(matrixB);
            free(matrixC);
            printf("\n");
        }
    }

    fclose(fp);

    return 0;
}