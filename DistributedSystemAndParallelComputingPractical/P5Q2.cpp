// P5Q1.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <omp.h>

#define MAX_EMPLOYEE 10000000
#define NUM_THREADS 4

enum class Co {
	Company1, Company2
};

int fetchTheSalary(const int employee,
	const Co company) {
	if (company == Co::Company1) {
		return (employee < 100) ? 99340 : 54300;
	}
	else {
		return (employee < 1000) ? 88770 : 57330;
	}
}

int serial() {
	int salaries1 = 0;
	int salaries2 = 0;
	double runtime;
	runtime = omp_get_wtime();
	for (int employee = 0; employee < MAX_EMPLOYEE; employee++) {
		salaries1 += fetchTheSalary(employee, Co::Company1);
	}
	std::cout << "Salaries1: " << salaries1 << std::endl;

	for (int employee = 0; employee < MAX_EMPLOYEE; employee++) {
		salaries2 += fetchTheSalary(employee, Co::Company2);
	}
	std::cout << "Salaries2: " << salaries2 << std::endl;

	runtime = omp_get_wtime() - runtime;
	std::cout << "In " << runtime << "seconds";
	return 0;
}

int parallel() {
	int salaries1 = 0;
	int salaries2 = 0;
	double runtime;

	runtime = omp_get_wtime();
	//omp_set_num_threads(NUM_THREADS);


#pragma omp parallel for reduction(+:salaries1, salaries2)
	for (int employee = 0; employee < MAX_EMPLOYEE; employee++) {
		salaries1 += fetchTheSalary(employee, Co::Company1);
		salaries2 += fetchTheSalary(employee, Co::Company2);
	}
	std::cout << "Salaries1: " << salaries1 << std::endl;
	std::cout << "Salaries2: " << salaries2 << std::endl;


	runtime = omp_get_wtime() - runtime;
	std::cout << "In " << runtime << "seconds";
	return 0;
}

int main() {
	serial();
	printf("\n");
	parallel();
}