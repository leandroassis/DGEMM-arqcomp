#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "DGEMM.h"

int main(int argc, char **argv){

    double **matrixA, **matrixB, **matrixC;
    int random_base;
    const unsigned int fixed_seed = 0x12345678;
    unsigned int size;
    size_t offset;

    if(argv[1] == NULL){
        printf("Uso: %s <tamanho-da-matriz> -f (opcional)\n", argv[0]);
        return 1;
    }
    else size = strtoul(argv[1], NULL, 10);

    // aloca memoria para as matrizes
    aloca_matrizes(size, matrixA, matrixB, matrixC);

    if(strncmp(argv[1], "-f", sizeof("-f"))) srand(fixed_seed);
    else srand(time(NULL));

    // inicializa as matrizes com valores aleatórios
    for(size_t i = 0; i < size; i++){
        random_base = rand();
        for(size_t j = 0; j < size; j++){
            matrixA[i][j] = rand()/random_base;
            matrixB[i][j] = rand()/random_base;
            matrixC[i][j] = rand()/random_base;
        }
    }

    // roda o algoritmo DGEMM
    dgemm(size, matrixA, matrixB, matrixC);

    return 0;

}