#include "utils/include/outUtils.hpp"

void outputMatrix(vector<vector<int> > matrix)
{
    UINT maxNumLength = getMaxNumberLengthInMatrix(matrix);

    for (UINT i = 0; i < matrix.size(); i++)
    {
        for (UINT j = 0; j < matrix.at(i).size(); j++)
        {
            cout << matrix.at(i).at(j) << getSpaces(maxNumLength * 2 - getNumberLength(matrix[i][j]));
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