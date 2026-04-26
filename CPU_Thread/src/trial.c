#include <stdio.h>
#include <time.h>
#include <omp.h>
#include "trial.h"
#include "matrix.h"
#include "util.h"

#define RUNTIME_SIZE 100

void runTrials_OMP(int *A, int *B, int *C, struct args_s args)
{
    double runtimes[RUNTIME_SIZE];
    omp_set_num_threads(args.num_threads);

    printf("Unblocked OpenMP runs:\n");
    for (int i = 0; i < args.num_trials; i++) {
        runtimes[i] = runDefaultVariant(A, B, C, matmul_openmp);
    }
    printf("Runtime average: %.1lf ms\n", calcMean(runtimes, args.num_trials));
    printf("Runtime Standard Deviation: %.1lf ms\n", calc_stdDev(runtimes, args.num_trials));

    printf("\nBlocked/Tiled OpenMP runs:\n");
    for (int i = 0; i < args.num_trials; i++) {
        runtimes[i] = runBlockedVariant(A, B, C, args.block_size, matmul_openmp_blocked);
    }
    printf("Runtime average: %.1lf ms\n", calcMean(runtimes, args.num_trials));
    printf("Runtime Standard Deviation: %.1lf ms\n", calc_stdDev(runtimes, args.num_trials));

    printf("\nDefault Matrix Multiplication:\n");
    runDefaultVariant(A, B, C, matmul);
}

void runTrials(int *A, int *B, int *C, struct args_s args)
{
    double runtimes[RUNTIME_SIZE] = {0};

    printf("\nBlocked/Tiled runs:\n");

    for (int i = 0; i < args.num_trials; i++) {
        runtimes[i] = runBlockedVariant(A, B, C, args.block_size, matmul_blocked);
    }
    printf("Runtime average: %.1lf ms\n", calcMean(runtimes, args.num_trials));
    printf("Runtime Standard Deviation: %.1lf ms\n", calc_stdDev(runtimes, args.num_trials));

    double defRuntime = runDefaultVariant(A, B, C, matmul);
    printf("\nmatmul() runtime: %.1lf ms\n", defRuntime);
    printf(" - Hash Value: %x\n", getMatrixHash(C, N));
}


double runDefaultVariant(int *A, int *B, int *C, void (*matmulFunc)(int*, int*, int*, int))
{
    struct timespec ts_start, ts_end;
    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    matmulFunc(A, B, C, N);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    double runtime = getTimeDifference(ts_start, ts_end);

    printf("Runtime: %.1lf ms\n", runtime);
    printf(" - Hash Value: %x\n", getMatrixHash(C, N));

    return runtime;
}

double runBlockedVariant(int *A, int *B, int *C, int blockSize, void (*matmulFunc)(int*, int*, int*, int, int))
{
    struct timespec ts_start, ts_end;
    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    matmulFunc(A, B, C, N, blockSize);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    double runtime = getTimeDifference(ts_start, ts_end);

    printf("Runtime: %.1lf ms\n", runtime);
    printf(" - Hash Value: %x\n", getMatrixHash(C, N));

    return runtime;
}

