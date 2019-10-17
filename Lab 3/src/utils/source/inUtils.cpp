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
