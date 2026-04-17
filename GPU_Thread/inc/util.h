#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <stdint.h>

float calc_stdDev(int data[], int n);
float calcMean(int data[], int n);
int getTimeDifference(struct timespec ts_start, struct timespec ts_end);
char matrixIsEqual(float *A, float *B, int n);
uint32_t SuperFastHash(const char *data, int len);

#endif
