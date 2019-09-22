#include "utils/include/auxUtils.hpp"

string getSpaces(int amount)
{
    string intermediateResult = "";

    for (UINT i = 0; i < amount; i++)
    {
        intermediateResult += " ";
    }

    return intermediateResult;
}

UINT getNumberLength(int number)
{
    UINT intermediateResult = 0;
    UINT i = 0;

    if (number == 0)
    {
        intermediateResult = 1;
        return intermediateResult;
    }
    else
    {
        while (floor(number / pow(10, i)) != 0)
        {
            intermediateResult++;
            i++;
        }
        return intermediateResult;
    }
}

UINT getMaxNumberLengthInMatrix(vector<vector<int>> matrix)
{
    UINT intermediateResult = 0;

    for (UINT i = 0; i < matrix.size(); i++)
    {
        for (UINT j = 0; i < matrix.at(i).size(); j++)
        {
            UINT tmpLength = getNumberLength(matrix.at(i).at(j));

            if (tmpLength > intermediateResult)
            {
                intermediateResult = tmpLength;
            }
        }
    }

    return intermediateResult;
}
