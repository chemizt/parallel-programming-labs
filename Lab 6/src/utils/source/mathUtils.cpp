#include "auxUtils.hpp"
#include "mathUtils.hpp"

int calculateVectorSum(vector<int> vector)
{
    int result = 0;

    for (uInt i = 0; i < vector.size(); i++)
    {
        result += vector.at(i);
    }

    return result;
}
