// P7Q3.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <omp.h>

void normal() {
	omp_set_num_threads(2);
	int a = 1;
	int b = 2;
	int c = 4;

#pragma omp parallel
	{
		a = a + b;
		printf("%i\n", a);
		printf("%i\n", b);
		printf("%i\n", c);
	}

	printf("Final = %d", a);
}


void firstprivate() {
	omp_set_num_threads(2);
	int a = 1;
	int b = 2;
	int c = 4;

#pragma omp parallel firstprivate(a)
	{
		a = a + b;
		printf("%i\n", a);
		printf("%i\n", b);
		printf("%i\n", c);
	}

	printf("Final = %d", a);
}

int main()
{
	normal();
	printf("\n\n\n");
	firstprivate();
	
	return 0;
}
