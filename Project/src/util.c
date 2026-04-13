#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void printMatrix(int **A, int n) 
{
    for (int i = 0; i < n; i++) {
        printf("|");
        for (int j = 0; j < n; j++) {
            printf("%4d, ", A[i][j]);
        }
        printf("\b\b|\n");
    }
}

char matrixIsEqual(int **A, int **B, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (A[i][j] != B[i][j]) return 0;
        }
    }
    return 1;
}

int** mallocMatrix(int n)
{
    int **matrix = malloc(n * sizeof(int *));
    int *tmp = malloc(n * n * sizeof(int));

    if (!matrix || !tmp) {
        perror("Matrix Memory allocation failed.");
        free(matrix);
        free(tmp);
        matrix = NULL;

        return NULL;
    }

    for (int r = 0; r < n; r++) {
        matrix[r] = tmp + r*n;
    }

    return matrix;
}

void freeMatrix(int **A)
{
    if (A) {
        free(*A);
        *A = NULL;
    }

    free(A);
}
