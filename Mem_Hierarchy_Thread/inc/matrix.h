#ifndef MATRIX_H
#define MATRIX_H

void matmul(int *A, int *B, int *C, int n);
void matmul_openmp(int *A, int *B, int *C, int n);
void matmul_blocked(int *A, int *B, int *C, int n, int BLOCK_SIZE);

#endif
