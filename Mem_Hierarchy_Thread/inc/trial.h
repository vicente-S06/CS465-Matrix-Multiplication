#ifndef TRIAL_H
#define TRIAL_H

#include "util.h"
#define N 1024

void runTrials(int *A, int *B, int *C, struct args_s args);
void runTrials_OMP(int *A, int *B, int *C, struct args_s args);
double runDefaultVariant(int *A, int *B, int *C, void (*matmulFunc)(int*, int*, int*, int));
double runBlockedVariant(int *A, int *B, int *C, int blockSize, void (*matmulFunc)(int*, int*, int*, int, int));

#endif
