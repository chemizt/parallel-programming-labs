#include "randUtils.hpp"

int getRandomUpTo(int maxValue)
{
    random_device rD;
    mt19937_64 rNG(rD());
    uniform_int_distribution<> distribution(0, maxValue);

    rNG.seed(rNG());

    return distribution(rNG);
}

vector<int> createRandFilledVector(uInt size)
{
    vector<int> intermediateResult;
    
    for (uInt i = 0; i < size; i++)
    {
        intermediateResult.push_back(getRandomUpTo(RAND_MAX_VALUE));
    }

    return intermediateResult;
}

intMatrix createRandFilledSquareMatrix(uInt size)
{
    intMatrix intermediateResult;

    for (uInt i = 0; i < size; i++)
    {
        intermediateResult.push_back(createRandFilledVector(size));
    }

    return intermediateResult;
}