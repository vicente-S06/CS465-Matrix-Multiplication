#include <cuda_runtime.h>
#include <stdlib.h>
#include "matrix.hpp"

__global__ void matmul_gpu_tiled(float *A, float *B, float *C, int n)
{
    // Assuming TILE_SIZE = blockDim.x = BlockDim.y
    __shared__ float tileA[TILE_SIZE][TILE_SIZE];
    __shared__ float tileB[TILE_SIZE][TILE_SIZE];

    int row = blockIdx.y * TILE_SIZE + threadIdx.y;
    int col = blockIdx.x * TILE_SIZE + threadIdx.x;

    float sum = 0.0f;

    for (int t = 0; t < (n + TILE_SIZE - 1) / TILE_SIZE; t++) {
        if (row < n && (t * TILE_SIZE + threadIdx.x) < n) {
            tileA[threadIdx.y][threadIdx.x] = A[row * n + t * TILE_SIZE + threadIdx.x];
        } else {
            tileA[threadIdx.y][threadIdx.x] = 0.0f;
        }

        
        if (col < n && (t * TILE_SIZE + threadIdx.y) < n) {
            tileB[threadIdx.y][threadIdx.x] = B[(t * TILE_SIZE + threadIdx.y) * n + col];
        } else {
            tileB[threadIdx.y][threadIdx.x] = 0.0f;
        }

        __syncthreads();

        for (int k = 0; k < TILE_SIZE; k++) {
            sum += tileA[threadIdx.y][k] * tileB[k][threadIdx.x];
        }

        __syncthreads();
    }
    
    if (row < n && col < n) {
        C[row * n + col] = sum;
    }
}

__global__ void matmul_gpu_tiled_2(float *A, float *B, float *C, int n)
{
    // Assuming TILE_SIZE = blockDim.x = BlockDim.y
    __shared__ float tileA[TILE_SIZE][TILE_SIZE];
    __shared__ float tileB[TILE_SIZE][TILE_SIZE];

    int row = blockIdx.y * TILE_SIZE + threadIdx.y;
    int col = blockIdx.x * TILE_SIZE + threadIdx.x;

    float sum = 0.0f;

    // gridDim.x == gridDim.y == # of blocks in 1 dimension 
    for (int t = 0; t < gridDim.x; t++) {
        // Each individual thread in a block reads a single value from A, and a single value from B.
        // (per iteration)
        // tileA will end up reading TILE_SIZE rows for each block.
        if (row < n && (t * TILE_SIZE + threadIdx.x) < n) {
            tileA[threadIdx.y][threadIdx.x] = A[row * n + t * TILE_SIZE + threadIdx.x];
        } else {
            tileA[threadIdx.y][threadIdx.x] = 0.0f;
        }

        
        // tileB will end up reading TILE_SIZE columns for each block.
        // Stores B columns as rows in tileB for better cache access
        if (col < n && (t * TILE_SIZE + threadIdx.y) < n) {
            tileB[threadIdx.x][threadIdx.y] = B[(t * TILE_SIZE + threadIdx.y) * n + col];
        } else {
            tileB[threadIdx.x][threadIdx.y] = 0.0f;
        }

        __syncthreads();

        for (int k = 0; k < TILE_SIZE; k++) {
            sum += tileA[threadIdx.y][k] * tileB[threadIdx.x][k];
        }

        __syncthreads();
    }
    
    if (row < n && col < n) {
        C[row * n + col] = sum;
    }
}


// Matrix multiplication to run on host
void matmul_blocked(float *A, float *B, float *C, int n, int blockSize)
{
    zeroMatrix(C, n);

    for (int blockI = 0; blockI < n; blockI += blockSize) { // Rows of A & C
        for (int blockK = 0; blockK < n; blockK += blockSize) { // Cols of A & Rows of B
            for (int blockJ = 0; blockJ < n; blockJ += blockSize) { // Cols of B & C
                                                                    
                for (int i = blockI; i < blockI + blockSize && i < n; i++) {
                    int ac_row = i*n;
                    for (int j = blockJ; j < blockJ + blockSize && j < n; j++) {
                        float sum = C[ac_row + j];
                        for (int k = blockK; k < blockK + blockSize && k < n; k++) {
                            sum += A[ac_row + k] * B[k*n + j];
                        }
                        C[ac_row + j] = sum;
                    }
                }
            }
        }
    }
}

void fillMatrix(float *A, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i*n + j] = (float)rand();
        }
    }
}

void zeroMatrix(float *A, int n) 
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i*n + j] = 0.0f;
        }
    }
}
