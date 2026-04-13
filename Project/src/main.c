#include <bits/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "matrix.h"

#define N 1024

int main(int argc, char *argv[])
{
    // Argument Handling
    if (argc != 2) {
        printf("Incorrect # of Arguments\n");
        printf("Correct Usage:\n\t%s [BLOCK_SIZE]\n", argv[0]);
        return 0;
    }
    int blockSize = atoi(argv[1]);
    if (!blockSize) {
        printf("Invalid Block Size. Must be > 0\n");
        return 0;
    }
    // Set random seed
    srand(10);

    // Allocate matrices.
    int **A, **B, **C, **D;
    A = mallocMatrix(N);
    B = mallocMatrix(N);
    C = mallocMatrix(N);
    D = mallocMatrix(N);
    if (!A || !B || !C || !D) {
        freeMatrix(A);
        freeMatrix(B);
        freeMatrix(C);
        freeMatrix(D);
    }

    // Fill matrices A and B
    fillMatrix(A, N);
    fillMatrix(B, N);

    long runtime = 0;

    struct timespec ts_start = {0};
    struct timespec ts_end = {0};

    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    matmul_blocked_2(A, B, C, N, blockSize);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);

    runtime = getTimeDifference(ts_start, ts_end);
    printf("Time for matmul_blocked_2(): %ldms\n", runtime);


    freeMatrix(A);
    freeMatrix(B);
    freeMatrix(C);
    freeMatrix(D);

    return 0;
}
