#ifndef UTIL_H
#define UTIL_H

void printMatrix(int **A, int n);
int** mallocMatrix(int n);
void freeMatrix(int **A);
char matrixIsEqual(int **A, int **B, int n);

#endif
