#include <iostream>
#include "mpi.h"

using std::cout;
using std::endl;

int main()
{
	MPI_Init(NULL, NULL);
	
	int commSize;
	int commRank;

	MPI_Comm_size(MPI_COMM_WORLD, &commSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

	cout << "Hello, MPI World! I am process with rank " << commRank << " among " << commSize << endl;

	MPI_Finalize();

	return 0;
}