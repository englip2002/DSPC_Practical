// P4Q2.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <cstdlib>
#include "omp.h"

#define NUM_THREADS 4
const long MAX = 100000;

int original() {
	double ave = 0.0, * A;
	int i;

	A = (double*)malloc(MAX * sizeof(double));
	if (A == NULL) {
		printf("Insufficient memory! Can't continue. Terminating the program abruptly.\n");
		return -1;
	}

	for (i = 0; i < MAX; i++)
	{
		A[i] = (double)i;
	}
	double start_time = omp_get_wtime();

	for (i = 0; i < MAX; i++) {
		ave += A[i];
	}
	double end_time = omp_get_wtime();
	ave = ave / MAX;
	printf("%f\n", ave);
	printf("Original work took %f seconds\n", end_time - start_time);
	free(A);
	return 0;
}

/// <summary>
/// Using pragma omp parallel, pragma omp critical
/// </summary>
/// <returns></returns>
int criticalRegion() {
	double ave = 0.0, * A;
	int i, nthreads;
	double sum= 0.0;

	omp_set_num_threads(NUM_THREADS);
	A = (double*)malloc(MAX * sizeof(double));
	if (A == NULL) {
		printf("Insufficient memory! Can't continue. Terminating the program abruptly.\n");
		return -1;
	}

	for (i = 0; i < MAX; i++)
	{
		A[i] = (double)i;
	}
	double start_time = omp_get_wtime();

#pragma omp parallel
	{
		int i, id, nthrds;
		double partial_sums;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();

		if (id == 0) {
			nthreads = nthrds;
			printf("Total OMP threads: %d\n", nthrds);
		}

		for (int i = id, partial_sums = 0.0; i < MAX; i += nthrds) {
			partial_sums += A[i];
		}

#pragma omp critical
		{
			sum += partial_sums;
		}
	}


	double end_time = omp_get_wtime();
	ave = sum / MAX;
	printf("%f\n", ave);
	printf("Critical work took %f seconds\n", end_time - start_time);
	free(A);
	return 0;
}


/// <summary>
/// Using pragma omp parallel for reduction(+:ave)
/// </summary>
/// <returns></returns>
int reduction() {
	double ave = 0.0, * A;
	int i;

	A = (double*)malloc(MAX * sizeof(double));
	if (A == NULL) {
		printf("Insufficient memory! Can't continue. Terminating the program abruptly.\n");
		return -1;
	}

	for (i = 0; i < MAX; i++)
	{
		A[i] = (double)i;
	}


	double start_time = omp_get_wtime();
	omp_set_num_threads(NUM_THREADS);
#pragma omp parallel 
	{
		int id, nthrds;
		double x;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0) {
			printf("Total OMP threads: %d\n", nthrds);
		}

		//use reduction to access ave in multiple thread
		//no need to have partial_sum to store temp data
#pragma omp parallel for reduction(+: ave)
		for (int i = id; i < MAX; i += nthrds) {
			ave += A[i];
		}
	}


	double end_time = omp_get_wtime();
	ave = ave / MAX;
	printf("%f\n", ave);
	printf("Reduction work took %f seconds\n", end_time - start_time);
	free(A);
	return 0;
}



int main()
{
	original();
	criticalRegion();
	reduction();
	return 0;
}