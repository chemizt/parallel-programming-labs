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
    intMatrix baseMatrix;
    vector<int> baseVector;
    vector<int> result;
    uInt worksetSize = std::stoi(argv[1]);

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    if (!processRank)
    {
        for (uInt i = 0; i < worksetSize; i++)
        {
            baseMatrix.push_back(createInitializedVector(worksetSize));
        }
        baseVector = createInitializedVector(worksetSize);
                    
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

    worksetSize = round(worksetSize / processCount);

    double start = MPI_Wtime();

    for (uInt i = 0; i < worksetSize; i++)
    {
        result.push_back(0);
        for (uInt j = 0; j < worksetSize; j++)
        {
            result.at(i) += baseMatrix.at(i).at(j) * baseVector.at(j);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

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
