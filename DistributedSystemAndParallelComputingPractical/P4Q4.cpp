// P4Q4.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "omp.h"

float work1(int i) {
    return 1.0 * i;
}

float work2(int i) {
    return 2.0 * i;
}
float Sum_array(int nval, float* A) {
    float sum = 0.0;
    for (int i = 0; i < nval; i++) sum = sum + A[i];
    return sum;
}

void original_atomic_example(float* x, float* y, int* index, int n) {
    int i;

#pragma omp parallel for shared(x, y, index, n)
    for (i = 0; i < n; i++) {
        x[index[i]] += work1(i);
        y[i] += work2(i);
    }
}

void modified_atomic_example(float* x, float* y, int* index, int n) {
    int i;

#pragma omp parallel for shared(x, y, index, n)
    for (i = 0; i < n; i++) {
        //when using atomic the result will remain same
    #pragma omp atomic
        x[index[i]] += work1(i);
        y[i] += work2(i);
    }
}

int original() {
    float x[1000];
    float y[10000];
    int index[10000];
    float sum1, sum2;
    int i;

    for (i = 0; i < 10000; i++) {
        index[i] = i % 1000;
        y[i] = 0.0;
    }
    for (i = 0; i < 1000; i++)
        x[i] = 0.0;
    original_atomic_example(x, y, index, 10000);

    sum1 = Sum_array(1000, x);
    sum2 = Sum_array(10000, y);

    printf("The sum is %f and %f\n", sum1, sum2);
    return 0;
}

int atomicConstruct() {
    float x[1000];
    float y[10000];
    int index[10000];
    float sum1, sum2;
    int i;

    for (i = 0; i < 10000; i++) {
        index[i] = i % 1000;
        y[i] = 0.0;
    }
    for (i = 0; i < 1000; i++)
        x[i] = 0.0;
    modified_atomic_example(x, y, index, 10000);

    sum1 = Sum_array(1000, x);
    sum2 = Sum_array(10000, y);

    printf("The sum is %f and %f\n", sum1, sum2);
    return 0;
}

int main() {
    printf("%s", "Original\n");
    printf("%s", "--------------------------\n");
    original();
    printf("%s", "\nAtomic Construct\n");
    printf("%s", "--------------------------\n");
    atomicConstruct();
}