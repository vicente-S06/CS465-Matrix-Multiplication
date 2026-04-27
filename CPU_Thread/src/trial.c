#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>
#include "trial.h"
#include "matrix.h"
#include "util.h"

#define RUNTIME_SIZE 100

void runTrials(int *A, int *B, int *C, struct args_s args)
{
    double runtimes[RUNTIME_SIZE] = {0};

    printf("\nBlocked/Tiled runs:\n");

    for (int i = 0; i < args.num_trials; i++) {
        runtimes[i] = runBlockedVariant(A, B, C, args.block_size, matmul_blocked);
    }
    printf("Runtime average: %.1lf ms\n", calcMean(runtimes, args.num_trials));
    printf("Runtime Standard Deviation: %.1lf ms\n", calc_stdDev(runtimes, args.num_trials));

    printf("\nDefault Matrix Multiplication:\n");
    runDefaultVariant(A, B, C, matmul);
}

typedef struct {
    volatile int *start_flag;
    int *matrixA;
    int *matrixB;
    int *matrixC;
    int start_row; // inclusive
    int end_row;   // exclusive
} ThreadData;

void* multiply(void *arg)
{
    ThreadData* data = (ThreadData*) arg;
    int *A = data->matrixA;
    int *B = data->matrixB;
    int *C = data->matrixC;

    // Wait until main thread signals start
    while (!*(data->start_flag));

    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i*N + k] * B[k*N + j];
            }
            C[i*N + j] = sum;
        }
    }
    return NULL;
}

void prepare_pthreads(int *A, int *B, int *C, volatile int *startf_ptr, ThreadData threadData[], struct args_s args)
{
    int rows_per_thread = N / args.num_threads;

    /* Prepare thread data */
    // Common thread data
    threadData[0].start_flag = startf_ptr;
    threadData[0].matrixA = A;
    threadData[0].matrixB = B;
    threadData[0].matrixC = C;

    // Differing thread Data
    for (int i = 0; i < args.num_threads; i++) {
        threadData[i] = threadData[0]; // Copy common data first.

        threadData[i].start_row = i * rows_per_thread;
        threadData[i].end_row = (i == args.num_threads - 1) ? N : (i+1) * args.num_threads;
    }
}

void runTrials_pthread(int *A, int *B, int *C, struct args_s args)
{
    double runtimes[RUNTIME_SIZE];
    struct timespec ts_start, ts_end;

    pthread_t threads[args.num_threads];
    ThreadData threadData[args.num_threads];
    volatile int start_flag = 0;

    prepare_pthreads(A, B, C, &start_flag, threadData, args);

#define TESTING
#ifndef TESTING
    printf("Unblocked pthreads runs:\n");
    for (int i = 0; i < args.num_trials; i++) {
        for (int t = 0; t < args.num_threads; t++) {
            pthread_create(&threads[t], NULL, multiply, &threadData[t]);
        }

        clock_gettime(CLOCK_MONOTONIC, &ts_start);

        start_flag = 1; // Start all threads.
                        
        for (int t = 0; t < args.num_threads; t++) {
            pthread_join(threads[t], NULL);
        }

        clock_gettime(CLOCK_MONOTONIC, &ts_end);

        runtimes[i] = getTimeDifference(ts_start, ts_end);
        printf("Runtime: %.1lf ms\n", runtimes[i]);
        printf(" - Hash Value: %x\n", getMatrixHash(C, N));

        start_flag = 0;
    }
#else 
    start_flag = 1;
    printf("Unblocked pthreads runs:\n");
    for (int i = 0; i < args.num_trials; i++) {
        clock_gettime(CLOCK_MONOTONIC, &ts_start);
        for (int t = 0; t < args.num_threads; t++) {
            pthread_create(&threads[t], NULL, multiply, &threadData[t]);
        }
                        
        for (int t = 0; t < args.num_threads; t++) {
            pthread_join(threads[t], NULL);
        }

        clock_gettime(CLOCK_MONOTONIC, &ts_end);

        runtimes[i] = getTimeDifference(ts_start, ts_end);
        printf("Runtime: %.1lf ms\n", runtimes[i]);
        printf(" - Hash Value: %x\n", getMatrixHash(C, N));
    }

#endif
    printf("Runtime average: %.1lf ms\n", calcMean(runtimes, args.num_trials));
    printf("Runtime Standard Deviation: %.1lf ms\n", calc_stdDev(runtimes, args.num_trials));


    /*
    printf("Unblocked pthreads runs:\n");
    for (int i = 0; i < args.num_trials; i++) {
        runtimes[i] = runDefaultVariant(A, B, C, matmul_openmp);
    }
    printf("Runtime average: %.1lf ms\n", calcMean(runtimes, args.num_trials));
    printf("Runtime Standard Deviation: %.1lf ms\n", calc_stdDev(runtimes, args.num_trials));

    printf("\nBlocked/Tiled pthreads runs:\n");
    for (int i = 0; i < args.num_trials; i++) {
        runtimes[i] = runBlockedVariant(A, B, C, args.block_size, matmul_openmp_blocked);
    }
    printf("Runtime average: %.1lf ms\n", calcMean(runtimes, args.num_trials));
    printf("Runtime Standard Deviation: %.1lf ms\n", calc_stdDev(runtimes, args.num_trials));
    */

    printf("\nDefault Matrix Multiplication:\n");
    runDefaultVariant(A, B, C, matmul);
}

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

