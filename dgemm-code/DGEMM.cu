// compile com nvcc .\DGEMM.cu -o saida -O3 -Xptxas -O3 -lineinfo -arch=sm_61 -use_fast_math
// verifique a arquitetura da sua GPU

#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>

#define SEED                 0x12345678

// tamanho da matriz
#define NUM_ITERACOES       5 // numero de iteracoes para cada tamanho de matriz
#define TAMANHO_MIN_MATRIZES 4096 // 4096 linhas e 4096 colunas = 16.777.216 células aprox 16MB
#define TAMANHO_MAX_MATRIZES 8192 // 8192 linhas e 8192 colunas = 67.108.864 células aprox 64MB
#define NUM_RESULTADOS_DESEJADOS 4
#define STEP_MATRIZES        (TAMANHO_MAX_MATRIZES-TAMANHO_MIN_MATRIZES)/NUM_RESULTADOS_DESEJADOS // deve ser multiplo de 32 assim como os tamanhos min e max das matrizes

// valores inseridos na matriz
#define MIN_DOUBLE           -100000.0
#define MAX_DOUBLE            100000.0

#define BLOCK_SIZE 4

double rand_double(){
    return (MIN_DOUBLE + (rand()*(MAX_DOUBLE - MIN_DOUBLE)/RAND_MAX));
}

// Função do kernel CUDA para multiplicação de matrizes com uso de "SIMD" (via paralelização de threads)
__global__ void DGEMM(double* A, double* B, double* C, int size) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < size && col < size) {
        double sum = 0.0f;

        for (int k = 0; k < size; k++) {
            sum += A[row * size + k] * B[k * size + col];
        }

        C[row * size + col] = sum;
    }
}


int main() {

    cudaEvent_t t_init, t_fim;

    cudaEventCreate(&t_init);
    cudaEventCreate(&t_fim);

    float t_exe;
    FILE *fp;

    srand(SEED);

    fp = fopen("resultadosCUDA.csv", "w"); // RESULTADOS
    fprintf(fp, "Tamanho da matriz;Tempo DGEMM\n");
    
    for(size_t size = TAMANHO_MIN_MATRIZES; size <= TAMANHO_MAX_MATRIZES; size+=STEP_MATRIZES){

        // Alocação e inicialização das matrizes no host (CPU)
        double* A = (double*) malloc(size*size*sizeof(double));
        double* C = (double*) malloc(size*size*sizeof(double));
        double* B = (double*) malloc(size*size*sizeof(double));

        // Alocação das matrizes no device (GPU)
        size_t matrixSize = size*size*sizeof(double);
        double* d_A, * d_B, * d_C;
        cudaMalloc((void**)&d_A, matrixSize);
        cudaMalloc((void**)&d_B, matrixSize);
        cudaMalloc((void**)&d_C, matrixSize);

        int i;
        for (i = 0; i < size*size; i++) {
            A[i] = (double) rand_double();
            B[i] = (double) rand_double();
            C[i] = (double) rand_double();
        }

        // Cópia das matrizes da CPU para a GPU
        cudaMemcpy(d_A, A, matrixSize, cudaMemcpyHostToDevice);
        cudaMemcpy(d_B, B, matrixSize, cudaMemcpyHostToDevice);

        for(int i = 0; i < NUM_ITERACOES; i++){
            fprintf(fp, "%zu;", size*size);

            // Configuração das dimensões do grid e dos blocos
            dim3 blockSize(BLOCK_SIZE, BLOCK_SIZE);
            dim3 gridSize((size + BLOCK_SIZE - 1) / BLOCK_SIZE, (size + BLOCK_SIZE - 1) / BLOCK_SIZE);

            // Chamada do kernel CUDA para multiplicação de matrizes

            cudaEventRecord(t_init); // tempo de inicio
            DGEMM<<<gridSize, blockSize>>>(d_A, d_B, d_C, size);
            cudaEventRecord(t_fim);
            
            cudaEventSynchronize(t_fim);
            cudaEventElapsedTime(&t_exe, t_init, t_fim);

            printf("Tempo de execução do algoritmo DGEMM normal com matriz de tamanho %.2f MB: %f s.\n", (float) (size*size/(1024*1024)), t_exe/1e6);
            fprintf(fp, "%f\n", t_exe);

            // Cópia do resultado da GPU para a CPU
            // nao é necessario copiar o resultado para a CPU
            //cudaMemcpy(C, d_C, matrixSize, cudaMemcpyDeviceToHost);

        }

        // Liberação da memória
        free(A);
        free(B);
        free(C);
        cudaFree(d_A);
        cudaFree(d_B);
        cudaFree(d_C);
    }

    fclose(fp);

    return 0;
}
