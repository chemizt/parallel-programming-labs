#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>

#include "mathUtils.hpp"
#include "typeDefs.hpp"

using std::vector;
using std::string;

void debugPrint(string str)
{
    std::cout << str << std::endl;
}

int main()
{
    int intermediateResult = 0;
    int arraySize;
    int processRank;
    MPI_Status status;
    MPI_Comm parentComm;

    MPI_Init(NULL, NULL);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_get_parent(&parentComm);

    MPI_Bcast(&arraySize, 1, MPI_INT, 0, parentComm);
    vector<int> matrixRow(arraySize);
	vector<int> baseVector(arraySize);
    MPI_Bcast(baseVector.data(), arraySize, MPI_INT, 0, parentComm);

    if (processRank <= arraySize)
    {
        do
        {
            intermediateResult = 0;
            // debugPrint("process with rank " + std::to_string(processRank) + " is waiting to receive a message"); 
            MPI_Recv(matrixRow.data(), arraySize, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, parentComm, &status);
            // debugPrint("process with rank " + std::to_string(processRank) + " received a message from " + std::to_string(status.MPI_SOURCE) + " with tag " + std::to_string(status.MPI_TAG));
            if (status.MPI_TAG > 0)
            {
                for (uInt i = 0; i < arraySize; i++)
                {
                    intermediateResult += matrixRow.at(i) * baseVector.at(i);
                }
                MPI_Send(&intermediateResult, 1, MPI_INT, 0, status.MPI_TAG, parentComm);
            }
        } 
        while (status.MPI_TAG > 0);
    }
    
    MPI_Finalize();
    return 0;
}
