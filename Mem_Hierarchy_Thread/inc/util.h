#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <stdint.h>

double calc_stdDev(double data[], int n);
double calcMean(double data[], int n);
double getTimeDifference(struct timespec ts_start, struct timespec ts_end);
int* mallocMatrix(int n);
void zeroMatrix(int *A, int n);
void fillMatrix(int *A, int n);
uint32_t SuperFastHash(const char *data, int len);
uint32_t getMatrixHash(int *A, int n);

#endif
