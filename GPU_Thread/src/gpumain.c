#include <bits/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "matrix.h"
#include <cuda_runtime.h>

#define N 1024

void runTrials(int **A, int **B, int **C, int blockSize, int numTrials);

int main(int argc, char *argv[])
{
    // Argument Handling
    if (argc != 2 && argc != 3) {
        printf("Incorrect # of Arguments\nCorrect Usage:\n");
        printf("\t%s [BLOCK_SIZE]\n", argv[0]);
        printf("\t%s [BLOCK_SIZE] [# of Trials]\n", argv[0]);
        printf("Default # of Trials is 1.\n");
        return 0;
    }

    int blockSize = atoi(argv[1]);
    if (!blockSize) {
        printf("Invalid Block Size. Must be > 0\n");
        return 0;
    }

    int numTrials = 1;
    if (argc == 3) {
        numTrials = atoi(argv[2]);
        numTrials = (numTrials != 0)?numTrials:1;
    }

    // Set random seed
    srand(10);

    // Allocate matrices.
    int **A, **B, **C;
    A = mallocMatrix(N);
    B = mallocMatrix(N);
    C = mallocMatrix(N);
    if (!A || !B || !C) {
        freeMatrix(A);
        freeMatrix(B);
        freeMatrix(C);
    }

    // Fill matrices A and B
    fillMatrix(A, N);
    fillMatrix(B, N);

    runTrials(A, B, C, blockSize, numTrials);

    freeMatrix(A);
    freeMatrix(B);
    freeMatrix(C);

    return 0;
}


void runTrials(int **A, int **B, int **C, int blockSize, int numTrials)
{
    int *runtimes = malloc(numTrials * sizeof(int));
    if (!runtimes) {
        perror("runtimes array malloc failed.");
        return;
    }

    struct timespec ts_start = {0};
    struct timespec ts_end = {0};

    for (int i = 0; i < numTrials; i++) {
        clock_gettime(CLOCK_MONOTONIC, &ts_start);
        matmul_blocked(A, B, C, N, blockSize);
        clock_gettime(CLOCK_MONOTONIC, &ts_end);
        runtimes[i] = getTimeDifference(ts_start, ts_end);
        printf("matmul_blocked() runtime: %d ms\n", runtimes[i]);

        zeroMatrix(C, N);
    }

    printf("Runtime average: %d ms\n", (int)calcMean(runtimes, numTrials));
    printf("Runtime Standard Deviation: %.1lf ms\n", calc_stdDev(runtimes, numTrials));
}
