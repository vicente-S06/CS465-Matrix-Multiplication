#ifndef UTIL_H
#define UTIL_H

#include <time.h>

double calc_stdDev(int data[], int n);
double calcMean(int data[], int n);
int getTimeDifference(struct timespec ts_start, struct timespec ts_end);
void printMatrix(int **A, int n);
int** mallocMatrix(int n);
void freeMatrix(int **A);
char matrixIsEqual(int **A, int **B, int n);

#endif
