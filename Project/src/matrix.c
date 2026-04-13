#include <stdlib.h>
#include "matrix.h"

void matmul(int **A, int **B, int **C, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// i k j ordering for outer loop
void matmul_blocked(int **A, int **B, int **C, int n, int blockSize)
{
    zeroMatrix(C, n);

    for (int blockI = 0; blockI < n; blockI += blockSize) { // Rows of A & C
        for (int blockK = 0; blockK < n; blockK += blockSize) { // Cols of A & Rows of B
            for (int blockJ = 0; blockJ < n; blockJ += blockSize) { // Cols of B & C

                for (int i = blockI; i < blockI + blockSize && i < n; i++) {
                    for (int k = blockK; k < blockK + blockSize && k < n; k++) {
                        int a = A[i][k];
                        for (int j = blockJ; j < blockJ + blockSize && j < n; j++) {
                            C[i][j] += a * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

// i k j ordering for outer loop and i j k for inner
void matmul_blocked_2(int **A, int **B, int **C, int n, int blockSize)
{
    zeroMatrix(C, n);

    for (int blockI = 0; blockI < n; blockI += blockSize) { // Rows of A & C
        for (int blockK = 0; blockK < n; blockK += blockSize) { // Cols of A & Rows of B
            for (int blockJ = 0; blockJ < n; blockJ += blockSize) { // Cols of B & C
                                                                    //
                for (int i = blockI; i < blockI + blockSize && i < n; i++) {
                    for (int j = blockJ; j < blockJ + blockSize && j < n; j++) {
                        int sum = C[i][j];
                        for (int k = blockK; k < blockK + blockSize && k < n; k++) {
                            sum += A[i][k] * B[k][j];
                        }
                        C[i][j] = sum;
                    }
                }
            }
        }
    }
}

// i j k ordering for outer loop
//void matmul_blocked_ijk(int **A, int **B, int **C, int n, int blockSize)
void matmul_blocked_3(int **A, int **B, int **C, int n, int blockSize)
{
    zeroMatrix(C, n);

    for (int blockI = 0; blockI < n; blockI += blockSize) { // Rows of A & C
        for (int blockJ = 0; blockJ < n; blockJ += blockSize) { // Cols of B & C 
            for (int blockK = 0; blockK < n; blockK += blockSize) { // Cols of A & Rows of B

                for (int i = blockI; i < blockI + blockSize && i < n; i++) {
                    for (int k = blockK; k < blockK + blockSize && k < n; k++) {
                        int a = A[i][k];
                        for (int j = blockJ; j < blockJ + blockSize && j < n; j++) {
                            C[i][j] += a * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

// i j k ordering for outer loop and inner
//void matmul_blocked_ijk_2(int **A, int **B, int **C, int n, int blockSize)
void matmul_blocked_4(int **A, int **B, int **C, int n, int blockSize)
{
    zeroMatrix(C, n);

    for (int blockI = 0; blockI < n; blockI += blockSize) { // Rows of A & C
        for (int blockJ = 0; blockJ < n; blockJ += blockSize) { // Cols of B & C 
            for (int blockK = 0; blockK < n; blockK += blockSize) { // Cols of A & Rows of B

                for (int i = blockI; i < blockI + blockSize && i < n; i++) {
                    for (int j = blockJ; j < blockJ + blockSize && j < n; j++) {
                        int sum = C[i][j];
                        for (int k = blockK; k < blockK + blockSize && k < n; k++) {
                            sum += A[i][k] * B[k][j];
                        }
                        C[i][j] = sum;
                    }
                }
            }
        }
    }
}
void fillMatrix(int **A, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = rand();
        }
    }
}

void zeroMatrix(int **A, int n) 
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = 0;
        }
    }
}
