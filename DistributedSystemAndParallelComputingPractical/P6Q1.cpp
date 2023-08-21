//run in cmd/powershell with mpiexec -n 4 MPI_Example
#include <iostream>
#include <mpi.h>

using namespace std;

int main(int args, char** argvs)
{
    char name[MPI_MAX_PROCESSOR_NAME];
    cout << "Hello World" << endl;
    int rank = 0, numOfProcess = 0, len = 0;
    //to start MPI environement
    MPI_Init(&args, &argvs);
    //to get current process id
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //to retrieve information about the name of processor 
    MPI_Get_processor_name(name, &len);
    //get number of processor
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcess);
    cout << "Hello World from process rank(number) " << rank << " from " << numOfProcess << " on " << name << endl;
    //to end execution of MPI environment
    MPI_Finalize();
    return 0;
}
