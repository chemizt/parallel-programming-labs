#include "outUtils.hpp"

void outputIntMatrix(intMatrix matrix)
{
    uInt maxNumLength = getMaxNumberLengthInIntMatrix(matrix);

    for (uInt i = 0; i < matrix.size(); i++)
    {
        for (uInt j = 0; j < matrix.at(i).size(); j++)
        {
            cout << matrix.at(i).at(j) << getSpaces(maxNumLength * 2 - getIntNumberLength(matrix.at(i).at(j)));
        }
        cout << "\n";
    }
    cout << "\n";
}

void outputIntVector(vector<int> vector)
{
    for (uInt i = 0; i < vector.size(); i++)
    {
        cout << vector.at(i) << " ";
    }
    cout << "\n\n";
}

void outputDoubleMatrix(doubleMatrix matrix)
{
    uInt maxNumLength = getMaxNumberLengthInDoubleMatrix(matrix);
    
    cout << fixed << setprecision(2);

    for (uInt i = 0; i < matrix.size(); i++)
    {
        for (uInt j = 0; j < matrix.at(i).size(); j++)
        {
            cout << matrix.at(i).at(j) << getSpaces(maxNumLength * 2 - getDoubleNumberLength(matrix.at(i).at(j)));
        }
        cout << "\n";
    }
    cout << "\n";
}

void outputDoubleVector(vector<double> vector)
{
    cout << fixed << setprecision(2);

    for (uInt i = 0; i < vector.size(); i++)
    {
        cout << vector.at(i) << " ";
    }
    cout << "\n\n";
}
