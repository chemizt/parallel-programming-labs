#include <mpi.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "typeDefs.hpp"
#include "auxUtils.hpp"
#include "randUtils.hpp"
#include "mathUtils.hpp"

using std::vector;
using std::ofstream;

#define MPI_WTIME_IS_GLOBAL TRUE

int main(int argc, char** argv)
{
    int processRank;
    int processCount;
    double start;
    double end;
    intMatrix baseMatrix;
    vector<int> baseVector;
    vector<int> result;
    uInt worksetSize = std::stoi(argv[1]);
    uInt threadsQuantity = std::stoi(argv[2]);

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    
    for (uInt i = 0; i < worksetSize; i++)
    {
        baseMatrix.push_back(createInitializedVector(worksetSize));
    }
    baseVector = createInitializedVector(worksetSize);
    result = createInitializedVector(worksetSize);

    if (!processRank)
    {
        #pragma omp parallel for collapse(2)
        for (uInt i = 0; i < worksetSize; i++)
        {
            for (uInt j = 0; j < worksetSize; j++)
            {
                baseMatrix.at(i).at(j) = getRandomUpTo(RAND_MAX_VALUE);
            }
        }
                    
        #pragma omp parallel for
        for (uInt i = 0; i < worksetSize; i++)
        {
            baseVector.at(i) = getRandomUpTo(RAND_MAX_VALUE);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    uInt recvSize = round(worksetSize / processCount);
    
    start = MPI_Wtime();
    MPI_Bcast(baseVector.data(), worksetSize, MPI_INT, processRank, MPI_COMM_WORLD);
    MPI_Bcast(baseMatrix.data(), worksetSize, MPI_INT, processRank, MPI_COMM_WORLD);

    #pragma omp parallel for collapse(2) num_threads(threadsQuantity)
    for (uInt i = processRank * recvSize; i < (processRank + 1) * recvSize; i++)
    {
        for (uInt j = 0; j < worksetSize; j++)
        {
            result.at(i) += baseMatrix.at(i).at(j) * baseVector.at(j);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    
    if (!processRank)
    {
        end = MPI_Wtime();

        ofstream multiplicaltionDuration;
        multiplicaltionDuration.open("time");
        multiplicaltionDuration.clear();
        multiplicaltionDuration << end - start;
        multiplicaltionDuration.close();
    }

    MPI_Finalize();
    return 0;
}
