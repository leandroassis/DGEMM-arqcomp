#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "DGEMM.h"

int main(int argc, char *argv[]){

    double *matrixA, *matrixB, *matrixC1, *matrixC2, *matrixC3;
    struct timespec t_ini = {0, 0}, t_fim = {0, 0};
    float t_exe;
    FILE *fp, *fp2;
    int suportaUnroll = flagUNROLL;
    double erroAVX = 0.0, erroUNROLL = 0.0, erroMedio = 0.0;

    // seeda o gerador de números aleatórios
    srand(SEED);

    if(argc < 2){
        printf("Erro, eh necessario especificar o arquivo de escrita dos resultados.\n");
        printf("Uso: ./dgemm <nome-arquivo-saida>\n");
        return 1;
    }

    // abre o arquivo para escrita
    fp = fopen(argv[1], "w"); // RESULTADOS
    fp2 = fopen("erros.csv", "a"); // ERROS

    //if(suportaUnroll) fprintf(fp, "Tamanho da matriz;Tempo DGEMM normal;Tempo DGEMM AVX; Tempo DGEMM AVX + UNROLL\n");

    for(size_t size = TAMANHO_MIN_MATRIZES; size <= TAMANHO_MAX_MATRIZES; size+=STEP_MATRIZES){

        // aloca memoria para as matrizes A, B e C se beneficiando da localidade espacial
        // pois os acessos à memória são agrupados
        aloca_matrizes(size*size, &matrixA, &matrixB, &matrixC1);
        aloca_matrizes(size*size, &matrixC2, &matrixC3, NULL);        

        // inicializa as matrizes com valores "aleatórios"
        inicializa_matrizes(size, matrixA, matrixB, matrixC1);
        inicializa_matrizes(size, NULL, NULL, matrixC2);
        inicializa_matrizes(size, NULL, NULL, matrixC3);

        for(int i = 0; i < NUM_ITERACOES; i++){
            fprintf(fp, "%zu;", size*size);

            // roda o algoritmo DGEMM normal
            clock_gettime(CLOCK_MONOTONIC, &t_ini); // tempo de inicio
            dgemm(size, matrixA, matrixB, matrixC1);
            clock_gettime(CLOCK_MONOTONIC, &t_fim); // tempo final
            t_exe = (t_fim.tv_sec - t_ini.tv_sec) + (t_fim.tv_nsec - t_ini.tv_nsec) / 1e9; // tempo de execução

            printf("Tempo de execução do algoritmo DGEMM normal com matriz de tamanho %.2f MB: %.6f s.\n", (float) (size*size/(1024*1024)), t_exe);
            fprintf(fp, "%.6f;", t_exe);

            // roda o algoritmo DGEMM otimizado com AVX
            clock_gettime(CLOCK_MONOTONIC, &t_ini);
            dgemm_AVX(size, matrixA, matrixB, matrixC2);
            clock_gettime(CLOCK_MONOTONIC, &t_fim);
            t_exe = (t_fim.tv_sec - t_ini.tv_sec) + (t_fim.tv_nsec - t_ini.tv_nsec) / 1e9; // tempo de execução

            printf("Tempo de execução do algoritmo DGEMM AVX com matriz de tamanho %.2f MB: %.6f s.\n", (float) (size*size/(1024*1024)), t_exe);
            fprintf(fp, "%.6f;", t_exe);

            if(suportaUnroll){
                // roda o algoritmo DGEMM otimizado com AVX e UNROLL
                clock_gettime(CLOCK_MONOTONIC, &t_ini);
                dgemm_AVX_unroll(size, matrixA, matrixB, matrixC3);
                clock_gettime(CLOCK_MONOTONIC, &t_fim);
                t_exe = (t_fim.tv_sec - t_ini.tv_sec) + (t_fim.tv_nsec - t_ini.tv_nsec) / 1e9; // tempo de execução

                printf("Tempo de execução do algoritmo DGEMM AVX + unroll com matriz de tamanho %.2f MB: %.6f s.\n", (float) (size*size/(1024*1024)), t_exe);
                fprintf(fp, "%.6f\n", t_exe);
            }
            else fprintf(fp, "N/A\n");

            // calcula o erro entre as matrizes C1, C2 e C3
            erroMedio = compara_matrizes(size, matrixC1, matrixC2, matrixC3, &erroAVX, &erroUNROLL);
            if(suportaUnroll) fprintf(fp2, "%zu;%.6f;%.6f;%.6f\n", size*size, erroMedio, erroAVX, erroUNROLL);
            else fprintf(fp2, "%zu;%.6f;%.6f;N/A\n", size*size, erroMedio, erroAVX);

            printf("\n");
        }

        free(matrixA);
        free(matrixB);
        free(matrixC1);
        free(matrixC2);
        free(matrixC3);
        printf("\n");
    }

    fclose(fp);
    fclose(fp2);

    return 0;
}
