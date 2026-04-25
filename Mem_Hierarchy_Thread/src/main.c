#include <bits/time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "util.h"
#include "matrix.h"
#include "trial.h"

int main(int argc, char *argv[])
{
    struct args_s args;
    char valid_args = handleArgs(argc, argv, &args);
    if (!valid_args) return 1;

    // Set random seed
    srand(10);

    // Allocate matrices.
    int *A, *B, *C;
    A = mallocMatrix(N);
    B = mallocMatrix(N);
    C = mallocMatrix(N);
    if (!A || !B || !C) {
        free(A); free(B); free(C);
        return 1;
    }

    // Fill matrices A and B
    fillMatrix(A, N);
    fillMatrix(B, N);

    runTrials(A, B, C, args.block_size, args.num_trials);

    free(A); free(B); free(C);
    A = NULL;
    B = NULL;
    C = NULL;

    return 0;
}

#define OPENMP

void runTrials(int *A, int *B, int *C, int blockSize, int numTrials)
{
    double runtimes[N] = {0};

    struct timespec ts_start, ts_end;

    for (int i = 0; i < numTrials; i++) {
        clock_gettime(CLOCK_MONOTONIC, &ts_start);

#ifdef OPENMP
        matmul_openmp(A, B, C, N);
#else
        matmul_blocked(A, B, C, N, blockSize);
#endif

        clock_gettime(CLOCK_MONOTONIC, &ts_end);
        runtimes[i] = getTimeDifference(ts_start, ts_end);

#ifdef OPENMP
        printf("matmul_openmp() runtime: %.1lf ms\n", runtimes[i]);
#else
        printf("matmul_blocked() runtime: %.1lf ms\n", runtimes[i]);
#endif

        printf("Hash Value: %x\n", getMatrixHash(C, N));
        zeroMatrix(C, N);
    }

    printf("Runtime average: %.1lf ms\n", calcMean(runtimes, numTrials));
    printf("Runtime Standard Deviation: %.1lf ms\n", calc_stdDev(runtimes, numTrials));

    runDefault(A, B, C);
}

void runDefault(int *A, int *B, int *C)
{
    struct timespec ts_start, ts_end;
    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    matmul(A, B, C, N);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    printf("matmul() runtime: %.1lf ms\n", getTimeDifference(ts_start, ts_end));
    printf("Hash Value: %x\n", getMatrixHash(C, N));
}

