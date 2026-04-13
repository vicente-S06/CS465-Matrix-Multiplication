#ifndef MATRIX_H
#define MATRIX_H

void matmul(int **A, int **B, int **C, int n);
void matmul_blocked(int **A, int **B, int **C, int n, int BLOCK_SIZE);
void zeroMatrix(int **A, int n);
void fillMatrix(int **A, int n);

#endif
