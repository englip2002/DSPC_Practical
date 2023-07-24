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
#pragma omp parallel shared(ave) 
	{
		int id, nthrds;
		double x;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0) {
			printf("Total OMP threads: %d\n", nthrds);
		}
		for (int i = id; i < MAX; i += nthrds) {
#pragma omp critical
			{
				ave += A[i];
			}
		}
	}


	double end_time = omp_get_wtime();
	ave = ave / MAX;
	printf("%f\n", ave);
	printf("Original work took %f seconds\n", end_time - start_time);
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

#pragma omp parallel for reduction(+: ave)
		for (int i = id; i < MAX; i += nthrds) {
			ave += A[i];
		}
	}


	double end_time = omp_get_wtime();
	ave = ave / MAX;
	printf("%f\n", ave);
	printf("Original work took %f seconds\n", end_time - start_time);
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