#ifndef MATRIX_H
#define MATRIX_H

#define TILE_SIZE 16
__global__ void matmul_gpu_tiled(float *A, float *B, float *C, int n);
__global__ void matmul_gpu_tiled_2(float *A, float *B, float *C, int n);
void matmul_blocked(float *A, float *B, float *C, int n, int BLOCK_SIZE);
void fillMatrix(float *A, int n);
void zeroMatrix(float *A, int n);

#endif
