#include "inUtils.hpp"

vector<int> createHandFilledVector(uInt size)
{
    vector<int> intermediateResult;

    for (uInt i = 0; i < size; i++)
    {
        int currentValue;
        
        cin >> currentValue;
        intermediateResult.push_back(currentValue);
    }

    return intermediateResult;
}

intMatrix createHandFilledSquareMatrix(uInt size)
{
    intMatrix intermediateResult;

    for (uInt i = 0; i < size; i++)
    {
        intermediateResult.push_back(createHandFilledVector(size));
    }

    return intermediateResult;
}