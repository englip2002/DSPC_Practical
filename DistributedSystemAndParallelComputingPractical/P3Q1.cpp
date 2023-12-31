#include "omp.h"
#include "stdio.h"
#include "stdlib.h"

void a() {
	//parallel region - only this part will run parallel
#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		printf("hello(%d)", ID);
		printf("world(%d)\n", ID);
	}
}


void b() {
	//share data
	int x = 5;
#pragma omp parallel
	{
		//share memory - result not same
		x = x + 1;
		printf("shared: X is %d\n", x);
	};
}

void c() {
	//private data
#pragma omp parallel
	{
		//reinitialized - result is same
		int x; x = 3;
		printf("private: X is %d\n", x);
	}
}

void d() {
	int x = 5;
#pragma omp parallel
	{
		x = x + 1; // dangerous - because using share data
		printf("private: x is %d\n", x);
	}
	printf("after: x is %d\n", x); // also dangerous
}

int main()
{
	a();
	b();
	c();
	d();
	return 0;
}
