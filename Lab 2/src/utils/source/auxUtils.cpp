#include "auxUtils.hpp"

string getSpaces(uInt amount)
{
    string intermediateResult = "";

    for (uInt i = 0; i < amount; i++)
    {
        intermediateResult += " ";
    }

    return intermediateResult;
}

uInt getNumberLength(int number)
{
    uInt intermediateResult = 0;
    uInt i = 0;

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

uInt getMaxNumberLengthInMatrix(intMatrix matrix)
{
    uInt intermediateResult = 0;

    for (uInt i = 0; i < matrix.size(); i++)
    {
        for (uInt j = 0; j < matrix.at(i).size(); j++)
        {
            uInt tmpLength = getNumberLength(matrix.at(i).at(j));

            if (tmpLength > intermediateResult)
            {
                intermediateResult = tmpLength;
            }
        }
    }

    return intermediateResult;
}

vector<int> createInitializedVector(uInt size)
{
    vector<int> intermediateResult;

    for (uInt i = 0; i < size; i++)
    {
        intermediateResult.push_back(0);
    }

    return intermediateResult;
}
