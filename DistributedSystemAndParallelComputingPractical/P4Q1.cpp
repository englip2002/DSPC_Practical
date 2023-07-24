#include <stdio.h>
#include "omp.h"
#define NUM_THREADS 4
static long num_steps = 100000;
double step;

void criticalRegion() {
	int i, nthreads;
	double pi = 0;
	double partial_sums, sum = 0.0;
	step = 1.0 / (double)num_steps;
	omp_set_num_threads(NUM_THREADS);
	double start_time = omp_get_wtime();

#pragma omp parallel
	{
		int i, id, nthrds;
		double x;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0) {
			nthreads = nthrds;
			printf("Total OMP threads: %d\n", nthreads);
		}

		// the i will never be same between loop handled by thread
		for (i = id; i < num_steps; i = i + nthrds)
		{
			x = (i + 0.5) * step;
			#pragma omp critical
			{
				sum += 4.0 / (1.0 + x * x);
			}
		}
	}

	pi = sum * step;
	double end_time = omp_get_wtime();

	printf("%f\n", pi);

	printf("Work took %f seconds\n", end_time - start_time);
}

int main() {
	criticalRegion();
	return 0;
}