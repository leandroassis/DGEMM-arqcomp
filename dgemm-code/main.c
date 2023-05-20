#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "DGEMM.h"

int main(int argc, char *argv[]){

    double *matrixA, *matrixB, *matrixC;
    struct timespec t_ini = {0, 0}, t_fim = {0, 0};
    float t_exe;
    FILE *fp, *fp2;
    int suportaUnroll = flagUNROLL, computaTempo = TEMPO;
    double erroAVX = 0.0, erroUNROLL = 0.0, erroMedio = 0.0;

    // seeda o gerador de números aleatórios
    srand(SEED);

    if(argc < 2){
        printf("Erro, eh necessario especificar o arquivo de escrita dos resultados.\n");
        printf("Uso: ./dgemm <nome-arquivo-saida>\n");
        return 1;
    }

    // abre o arquivo para escrita
    fp = fopen(argv[1], "w");

    if(suportaUnroll) fprintf(fp, "Tamanho da matriz;Tempo DGEMM normal;Tempo DGEMM AVX; Tempo DGEMM AVX + UNROLL\n");
    else fprintf(fp, "Tamanho da matriz;Tempo DGEMM normal;Tempo DGEMM AVX\n");
    
    // compara os erros médios das diferentes formas de otimização para uma matriz pequena
    erroMedio = compara_matrizes(512, &erroAVX, &erroUNROLL);
    
    fp2 = fopen("erros.csv", "a");
    fprintf(fp2, "Erro médio;Erro AVX;Erro UNROLL\n");
    fprintf(fp2, "%f;%f;%f\n", erroMedio, erroAVX, erroUNROLL);
    fclose(fp2);

    if(!computaTempo){
	    fclose(fp);
	    return 0;
    }

    for(size_t size = TAMANHO_MIN_MATRIZES; size <= TAMANHO_MAX_MATRIZES; size+=STEP_MATRIZES){

        // aloca memoria para as matrizes A, B e C se beneficiando da localidade espacial
        // pois os acessos à memória são agrupados
        aloca_matrizes(size*size, &matrixA, &matrixB, &matrixC);

        // inicializa as matrizes com valores "aleatórios"
        inicializa_matrizes(size, matrixA, matrixB, matrixC);

        for(int i = 0; i < NUM_ITERACOES; i++){
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

            printf("Tempo de execução do algoritmo DGEMM AVX com matriz de tamanho %.2f MB: %.6f s.\n", (float) (size*size/(1024*1024)), t_exe);
            fprintf(fp, "%.6f;", t_exe);

            if(suportaUnroll){
                // roda o algoritmo DGEMM otimizado com AVX e UNROLL
                clock_gettime(CLOCK_MONOTONIC, &t_ini);
                dgemm_AVX_unroll((int) size, matrixA, matrixB, matrixC);
                clock_gettime(CLOCK_MONOTONIC, &t_fim);
                t_exe = (t_fim.tv_sec - t_ini.tv_sec) + (t_fim.tv_nsec - t_ini.tv_nsec) / 1e9; // tempo de execução

                printf("Tempo de execução do algoritmo DGEMM AVX + unroll com matriz de tamanho %.2f MB: %.6f s.\n", (float) (size*size/(1024*1024)), t_exe);
                fprintf(fp, "%.6f\n", t_exe);
            }
            else fprintf(fp, "\n");

            printf("\n");
        }

        free(matrixA);
        free(matrixB);
        free(matrixC);
        printf("\n");
    }

    fclose(fp);

    return 0;
}
