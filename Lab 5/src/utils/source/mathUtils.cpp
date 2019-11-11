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

uInt calculateWorksetSize(uInt multiplier)
{
    return WORKSET_BASE_SIZE + multiplier * WORKSET_SIZE_STEP;
}

uInt calculateMaxWorksetSize()
{
    return WORKSET_BASE_SIZE + WORKSET_STEP_COUNT * WORKSET_SIZE_STEP;
}

uInt min(uInt x, uInt y)
{
    return x < y ? x : y;
}

uInt max(uInt x, uInt y)
{
    return x > y ? x : y;
}