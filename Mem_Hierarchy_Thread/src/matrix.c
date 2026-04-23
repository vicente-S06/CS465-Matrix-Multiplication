#include "matrix.h"
#include "util.h"

void matmul(int *A, int *B, int *C, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[i*n + k] * B[k*n + j];
            }
            C[i*n + j] = sum;
        }
    }
}

void matmul_openmp(int *A, int *B, int *C, int n)
{
#pragma omp parallel for collapse(2) num_threads(16)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[i*n + k] * B[k*n + j];
            }
            C[i*n + j] = sum;
        }
    }
}

// i k j ordering for outer loop and i j k for inner
void matmul_blocked(int *A, int *B, int *C, int n, int blockSize)
{
    zeroMatrix(C, n);

    for (int blockI = 0; blockI < n; blockI += blockSize) { // Rows of A & C
        for (int blockK = 0; blockK < n; blockK += blockSize) { // Cols of A & Rows of B
            for (int blockJ = 0; blockJ < n; blockJ += blockSize) { // Cols of B & C
                                                                    
                for (int i = blockI; i < blockI + blockSize && i < n; i++) {
                    for (int j = blockJ; j < blockJ + blockSize && j < n; j++) {
                        int sum = C[i*n + j];
                        for (int k = blockK; k < blockK + blockSize && k < n; k++) {
                            sum += A[i*n + k] * B[k*n + j];
                        }
                        C[i*n + j] = sum;
                    }
                }
            }
        }
    }
}
