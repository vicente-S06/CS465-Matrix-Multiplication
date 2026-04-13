#include <stdlib.h>
#include <stdio.h>
#include "util.h"

#include <stdio.h>
#include <math.h>

/*
 * Calculates the sample standard deviation of an array of ints.
 * `data` - Array of input values
 * `n` - Number of elements in the array
 * `Returns:` The calculated standard deviation
 */
double calc_stdDev(int data[], int n)
{
    double variance = 0.0;
    // Get mean
    double mean = calcMean(data, n);

    // Calculate sum of squared differences
    for (int i = 0; i < n; ++i) {
        variance += pow(data[i] - mean, 2);
    }

    // Return the square root of the variance
    // For sample SD: variance / (n - 1)
    return sqrt(variance / (n - 1));
}

double calcMean(int data[], int n)
{
    int result = 0;
    for (int i = 0; i < n; ++i) {
        result += data[i];
    }

    return (double)result / n;
}

// returns time difference in milliseconds
int getTimeDifference(struct timespec ts_start, struct timespec ts_end)
{
    //nanoseconds to milliseconds
    #define TIME_SHIFT 1000000

    long startT = ts_start.tv_sec*1000 + ts_start.tv_nsec/TIME_SHIFT;
    long endT = ts_end.tv_sec*1000 + ts_end.tv_nsec/TIME_SHIFT;

    return endT - startT;
}

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
