// P4Q3.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <cstdlib>
#include "omp.h"

const int N = 100000;

void fill_rand(int nval, double* A) {
    for (int i = 0; i < nval; i++) A[i] = (double)rand();
}

double sum_array(int nval, double* A) {
    double sum = 0.0;
    for (int i = 0; i < nval; i++)
        sum = sum + A[i];
    return sum;
}

int original() {
    double* A, sum, runtime;	int flag = 0;

    A = (double*)malloc(N * sizeof(double));
    srand(100);
    runtime = omp_get_wtime();
    fill_rand(N, A);	// Producer: fill an array of data 
    sum = sum_array(N, A);	// Consumer: sum the array 
    runtime = omp_get_wtime() - runtime;

    printf("In %lf seconds, The sum is %lf \n", runtime, sum);

    return 0;
}

int parallel() {
    double* A, sum, runtime;	
    int flag = 0;

    A = (double*)malloc(N * sizeof(double));
    srand(100);
    
    runtime = omp_get_wtime();
#pragma omp parallel sections num_threads(2)
    {
    #pragma omp section
        {
            fill_rand(N, A);	// Producer: fill an array of data
            //flush to refresh the variable
            #pragma omp flush
            {
            flag = 1;
            #pragma omp flush(flag)
            }
        }
    #pragma omp section
        {
            #pragma omp flush(flag)
            while (flag != 1) {
                //printf("%s", "wait");
                #pragma omp flush(flag)
            }
            #pragma omp flush
            sum = sum_array(N, A);	// Consumer: sum the array
        }
    }
    runtime = omp_get_wtime() - runtime;
    printf("In %lf seconds, The sum is %lf \n", runtime, sum);

    return 0;
}

int main() {
    original();
    parallel();
    return 0;
}
