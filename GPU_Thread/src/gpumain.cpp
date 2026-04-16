#include <bits/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "matrix.hpp"
#include <cuda_runtime.h>

#define N 1024
#define NUM_TRIALS 4
#define CPU_BLOCK_SIZE 64

void runTrials(float *A, float *B, float *C);
uint32_t getMatrixHash(float *A);

int main()
{
    // Set random seed
    srand(10);
    float *A, *B, *C;
    float *d_A, *d_B, *d_C;
    //float *compareC;

    int allocSize = N*N * sizeof(float);

    // Allocate Host Memory using cudaMallocHost API. This is best practice
    // when buffers will be used for copies between CPU and GPU memory
    cudaMallocHost(&A, allocSize);
    cudaMallocHost(&B, allocSize);
    cudaMallocHost(&C, allocSize);
    //compareC = (float *)malloc(allocSize);

    // Fill matrices A and B
    fillMatrix(A, N);
    fillMatrix(B, N);

    struct timespec ts_start = {0};
    struct timespec ts_end = {0};
    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    matmul_blocked(A, B, C, N, CPU_BLOCK_SIZE);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    printf("matmul_blocked() runtime: %d ms\n", getTimeDifference(ts_start, ts_end));
    printf("Hash Value: %x\n", getMatrixHash(C));

    cudaMalloc(&d_A, allocSize);
    cudaMalloc(&d_B, allocSize);
    cudaMalloc(&d_C, allocSize);

    // Copy data to the GPU
    cudaMemcpy(d_A, A, allocSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, allocSize, cudaMemcpyHostToDevice);
    cudaMemset(d_C, 0, allocSize);

    runTrials(d_A, d_B, d_C);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    cudaFreeHost(A);
    cudaFreeHost(B);
    cudaFreeHost(C);
    //free(compareC);

    return 0;
}

void runTrials(float *A, float *B, float *C)
{
    int runtimes[NUM_TRIALS] = {0};
    struct timespec ts_start = {0};
    struct timespec ts_end = {0};

    int blockSize = (N + TILE_SIZE - 1) / TILE_SIZE;
    dim3 grid(blockSize, blockSize);
    dim3 block(TILE_SIZE, TILE_SIZE);

    for (int i = 0; i < NUM_TRIALS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &ts_start);
        matmul_gpu_tiled<<<grid, block>>>(A, B, C, N);
        cudaDeviceSynchronize();
        clock_gettime(CLOCK_MONOTONIC, &ts_end);
        runtimes[i] = getTimeDifference(ts_start, ts_end);
        printf("matmul_gpu_tiled() runtime: %d ms\n", runtimes[i]);

        zeroMatrix(C, N);
    }

    printf("Runtime average: %d ms\n", (int)calcMean(runtimes, NUM_TRIALS));
    printf("Runtime Standard Deviation: %.1lf ms\n", calc_stdDev(runtimes, NUM_TRIALS));
}

uint32_t getMatrixHash(float *A) 
{
    const char *data = (const char *) A;
    return SuperFastHash(data, N*N*sizeof(float));
}
