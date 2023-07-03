#include "omp.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
	//parallel processing

	//parallel region - only this part will run parallel
#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		printf("hello(%d)", ID);
		printf("world(%d)\n", ID);
	}
	return 0;
}
