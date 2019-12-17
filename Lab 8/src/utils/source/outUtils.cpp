#include "outUtils.hpp"

struct Comma final : std::numpunct<char> // inspired by (copy-typed from) https://stackoverflow.com/a/42331536
{
    char do_decimal_point() const override { return ','; }
};

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

void logOutput(string message)
{
    ofstream resultsFile;
    string timeStamp = getCurrentTimeStampAsString();

    resultsFile.open(LOG_FILE_NAME, std::ios_base::app);
    cout << timeStamp << ": " << message << endl;
    resultsFile << timeStamp << ": " << message << endl;
    resultsFile.close();
}

template <typename type>
void outputMatrixToFile(vector<vector<type>> matrix, string fileName)
{
    ofstream resultsFile;
    resultsFile.open(fileName, std::ios_base::app);
    resultsFile.imbue(locale(locale::classic(), new Comma));
    
    for (uInt i = 0; i < matrix.size(); i++)
    {
        for (uInt j = 0; j < matrix.at(i).size(); j++)
        {
            if (j < matrix.at(i).size() - 1)
            {
                resultsFile << matrix.at(i).at(j) << ";";
            }
            else
            {
                resultsFile << matrix.at(i).at(j) << "\n";
            }
        }
    }

    resultsFile << "\n\n";

    resultsFile.close();
}

template <typename type>
void outputVectorToFile(vector<type> vector, string fileName)
{
    ofstream resultsFile;
    resultsFile.open(fileName, std::ios_base::app);
    resultsFile.imbue(locale(locale::classic(), new Comma));
    
    for (uInt i = 0; i < vector.size(); i++)
    {
        if (i < vector.size() - 1)
        {
            resultsFile << vector.at(i) << ";";
        }
        else
        {
            resultsFile << vector.at(i) << "\n";
        }
    }

    resultsFile << "\n\n";

    resultsFile.close();
}