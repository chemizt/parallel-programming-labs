#include <iostream>
#include <vector>

#include "utils/include/auxUtils.hpp"
#include "utils/include/typeDefs.hpp"
#include "utils/include/outUtils.hpp"
#include "utils/include/randUtils.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

#define RAND_MAX_VALUE 1000

int main()
{
    setlocale(LC_CTYPE, "RU");
    vector<vector<int> > a;
    vector<int> b;
    vector<int> c;
    UINT size;

    cout << "Введите желаемую размерность матрицы/вектора: ";
    cin >> size;

    for (UINT i = 0; i < size; i++)
    {
        vector<int> tmp;
        a.push_back(tmp);
    }

    for (UINT i = 0; i < size; i++)
    {
        for (UINT j = 0; j < size; j++)
        {
            a.at(i).push_back(getRandomUpTo(RAND_MAX_VALUE));
        }
        b.push_back(getRandomUpTo(RAND_MAX_VALUE));
        c.push_back(0);
    }

    cout << "Исходная матрица:\n";
    outputMatrix(a);
    cout << "Исходный вектор:\n";
    outputVector(b);

    for (UINT i = 0; i < size; i++)
    {
        for (UINT j = 0; j < size; j++)
        {
            c.at(i) += a.at(i).at(j) * b.at(i);
        }
    }

    cout << "Результирующий вектор:\n";
    outputVector(c);

    return 0;
}