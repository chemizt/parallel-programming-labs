#include "auxUtils.hpp"
#include "mathUtils.hpp"

vector<int> multiplyMatrixAndVector(intMatrix baseMatrix, vector<int> baseVector)
{
    vector<int> intermediateResult;

    intermediateResult = createInitializedVector(baseVector.size());

    for (uInt i = 0; i < baseMatrix.size(); i++)
    {
        for (uInt j = 0; j < baseMatrix.at(i).size(); j++)
        {
            intermediateResult.at(i) += baseMatrix.at(i).at(j) * baseVector.at(j);
        }
    }

    return intermediateResult;
}