#include <mpi.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "typeDefs.hpp"
#include "auxUtils.hpp"
#include "randUtils.hpp"

using std::vector;
using std::ofstream;

#define MPI_WTIME_IS_GLOBAL TRUE

int main(int argc, char** argv)
{
    int processRank;
    int processCount;
    double start;
    double end;
    vector<int> firstBaseVector;
    vector<int> secondBaseVector;
    int result = 0;
    int allResult = 0;
    uInt worksetSize = std::stoi(argv[1]);

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    
    firstBaseVector = createInitializedVector(worksetSize);
    secondBaseVector = createInitializedVector(worksetSize);

    if (!processRank)
    {
        #pragma omp parallel for
        for (uInt i = 0; i < worksetSize; i++)
        {
            firstBaseVector.at(i) = getRandomUpTo(RAND_MAX_VALUE);
            secondBaseVector.at(i) = getRandomUpTo(RAND_MAX_VALUE);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    uInt recvSize = round(worksetSize / (processCount - 1));
    MPI_Bcast(firstBaseVector.data(), worksetSize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(secondBaseVector.data(), worksetSize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Win mpiWin;
    MPI_Win_create(firstBaseVector.data(), 1, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &mpiWin);
    
    start = MPI_Wtime();
    if (processRank)
    {
        for (uInt i = (processRank - 1) * recvSize; i < processRank * recvSize; i++)
        {
            MPI_Win_lock(MPI_LOCK_EXCLUSIVE, processRank, 0, mpiWin);
            result += firstBaseVector[i] + secondBaseVector[i];
            MPI_Win_unlock(processRank, mpiWin);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (!processRank)
    {
        ofstream multiplicaltionDuration;
        multiplicaltionDuration.open("time");
        multiplicaltionDuration.clear();
        multiplicaltionDuration << end - start;
        multiplicaltionDuration.close();
    }

    MPI_Finalize();
    return 0;
}
