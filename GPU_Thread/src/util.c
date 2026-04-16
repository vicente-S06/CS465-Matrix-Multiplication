#include <stdlib.h>
#include <stdio.h>
#include "util.h"

#include <stdio.h>
#include <math.h>

/*
 * Calculates the sample standard deviation of an array of floats.
 * `data` - Array of input values
 * `n` - Number of elements in the array
 * `Returns:` The calculated standard deviation
 */
float calc_stdDev(int data[], int n)
{
    double variance = 0.0;
    // Get mean
    double mean = calcMean(data, n);

    // Calculate sum of squared differences
    for (int i = 0; i < n; ++i) {
        variance += pow(data[i] - mean, 2);
    }

    // Return the square root of the variance
    // For sample SD: variance / (n - 1)
    return sqrt(variance / (n - 1));
}

float calcMean(int data[], int n)
{
    float result = 0;
    for (int i = 0; i < n; ++i) {
        result += data[i];
    }

    return result / n;
}

// returns time difference in milliseconds
int getTimeDifference(struct timespec ts_start, struct timespec ts_end)
{
    //nanoseconds to milliseconds
    #define TIME_SHIFT 1000000

    long startT = ts_start.tv_sec*1000 + ts_start.tv_nsec/TIME_SHIFT;
    long endT = ts_end.tv_sec*1000 + ts_end.tv_nsec/TIME_SHIFT;

    return endT - startT;
}

#define FLOAT_TOLERANCE 0.00001
char matrixIsEqual(float *A, float *B, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float difference = A[i*n + j] - B[i*n + j];
            if (difference < 0) difference = -difference;

            if (difference > FLOAT_TOLERANCE) return 0;
        }
    }
    return 1;
}

#include <stdint.h>
#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

uint32_t SuperFastHash(const char *data, int len)
{
    uint32_t hash = len, tmp;
    int rem;

    if (len <= 0 || data == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) {
        hash += get16bits(data);
        tmp = (get16bits(data+2) << 11) ^ hash;
        hash = (hash << 16) ^ tmp;
        data += 2 * sizeof(uint16_t);
        hash += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3:
            hash += get16bits(data);
            hash ^= hash << 16;
            hash ^= ((signed char) data[sizeof(uint16_t)]) << 18;
            hash += hash >> 11;
            break;
        case 2:
            hash += get16bits(data);
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        case 1:
            hash += (signed char)*data;
            hash ^= hash << 10;
            hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}
