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

uInt getIntNumberLength(int number)
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

uInt getDoubleNumberLength(double number)
{
    ostringstream strStream;
    strStream << fixed << setprecision(2) << number;
    return strStream.str().length();
}

uInt getMaxNumberLengthInIntMatrix(intMatrix matrix)
{
    uInt intermediateResult = 0;

    for (uInt i = 0; i < matrix.size(); i++)
    {
        for (uInt j = 0; j < matrix.at(i).size(); j++)
        {
            uInt tmpLength = getIntNumberLength(matrix.at(i).at(j));

            if (tmpLength > intermediateResult)
            {
                intermediateResult = tmpLength;
            }
        }
    }

    return intermediateResult;
}

uInt getMaxNumberLengthInDoubleMatrix(doubleMatrix matrix)
{
    uInt intermediateResult = 0;

    for (uInt i = 0; i < matrix.size(); i++)
    {
        for (uInt j = 0; j < matrix.at(i).size(); j++)
        {
            uInt tmpLength = getDoubleNumberLength(matrix.at(i).at(j));
            
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

vector<double> calculateAverageAccelFromDoubleMatrix(doubleMatrix matrix)
{
    double tmpSum;
    vector<double> intermediateResult;

    for (uInt i = 1; i < matrix.size(); i++)
    {
        tmpSum = 0;

        for (uInt j = 0; j < matrix.at(i).size(); j++)
        {
            tmpSum += matrix.at(i).at(j);
        }

        intermediateResult.push_back(tmpSum / matrix.at(i).size());
    }

    return intermediateResult;
}

string getCurrentTimeStampAsString() // copy-typed from https://stackoverflow.com/a/35157784
{
    auto timeStamp = system_clock::now();
    auto ms = duration_cast<milliseconds>(timeStamp.time_since_epoch()) % 1000;
    auto timer = system_clock::to_time_t(timeStamp);
    auto bt = *localtime(&timer);
    std::ostringstream oss;
    
    oss << put_time(&bt, "%H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}
