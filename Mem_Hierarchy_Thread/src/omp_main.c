#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "trial.h"

char handleArgs(int argc, char *argv[], struct args_s *args);

int main(int argc, char *argv[])
{
    // Set random seed
    srand(10);

    // Handle arguments
    struct args_s args;
    char valid_args = handleArgs(argc, argv, &args);
    if (!valid_args) return 1;


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

    runTrials_OMP(A, B, C, args);

    free(A); free(B); free(C);
    A = B = C = NULL;

    return 0;
}

char handleArgs(int argc, char *argv[], struct args_s *args)
{
    // Argument Handling
    if (argc != 3 && argc != 4) {
        printf("Incorrect # of Arguments\nCorrect Usage:\n");
        printf("\t%s [BLOCK_SIZE] [THREAD_COUNT]\n", argv[0]);
        printf("\t%s [BLOCK_SIZE] [THREAD_COUNT] [# of Trials]\n", argv[0]);
        printf("Default # of Trials is 1.\n");
        return 0;
    }

    args->block_size = atoi(argv[1]);
    if (!args->block_size) {
        printf("Invalid Block Size. Must be > 0\n");
        return 0;
    }

    args->num_threads = atoi(argv[2]);
    if (!args->num_threads) {
        printf("Invalid Thread Count. Must be > 0\n");
        return 0;
    }

    int tmp = 1;
    if (argc == 4) {
        tmp = atoi(argv[3]);
        if (tmp == 0) tmp = 1;
    }
    args->num_trials = tmp;

    return 1;
}
