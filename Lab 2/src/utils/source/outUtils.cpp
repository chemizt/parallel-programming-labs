#include "outUtils.hpp"

void outputMatrix(intMatrix matrix)
{
    UINT maxNumLength = getMaxNumberLengthInMatrix(matrix);

    for (UINT i = 0; i < matrix.size(); i++)
    {
        for (UINT j = 0; j < matrix.at(i).size(); j++)
        {
            cout << matrix.at(i).at(j) << getSpaces(maxNumLength * 2 - getNumberLength(matrix.at(i).at(j)));
        }
        cout << "\n";
    }
}

void outputVector(vector<int> vector)
{
    for (UINT i = 0; i < vector.size(); i++)
    {
        cout << vector.at(i) << " ";
    }
    cout << "\n";
}