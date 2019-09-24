#include "outUtils.hpp"

void outputMatrix(intMatrix matrix)
{
    uInt maxNumLength = getMaxNumberLengthInMatrix(matrix);

    for (uInt i = 0; i < matrix.size(); i++)
    {
        for (uInt j = 0; j < matrix.at(i).size(); j++)
        {
            cout << matrix.at(i).at(j) << getSpaces(maxNumLength * 2 - getNumberLength(matrix.at(i).at(j)));
        }
        cout << "\n";
    }
    cout << "\n";
}

void outputVector(vector<int> vector)
{
    for (uInt i = 0; i < vector.size(); i++)
    {
        cout << vector.at(i) << " ";
    }
    cout << "\n\n";
}