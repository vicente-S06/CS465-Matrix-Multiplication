#ifndef MATRIX_H
#define MATRIX_H

void fillMatrix(int **A, int n);
void matmul(int **A, int **B, int **C, int n);
void zeroMatrix(int **A, int n);
void matmul_blocked(int **A, int **B, int **C, int n, int BLOCK_SIZE);
void matmul_blocked_2(int **A, int **B, int **C, int n, int blockSize);
void matmul_blocked_3(int **A, int **B, int **C, int n, int blockSize);
void matmul_blocked_4(int **A, int **B, int **C, int n, int blockSize);

#endif
