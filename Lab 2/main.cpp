#include <iostream>
#include <vector>
#include <random>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::random_device;
using std::mt19937_64;
using std::uniform_int_distribution;

#define RAND_MAX_VALUE 1000

int getRandomUpTo(int maxValue)
{
    random_device rD;
    mt19937_64 rNG(rD());
    uniform_int_distribution<> distribution(0, maxValue);

    rNG.seed(rNG());

    return distribution(rNG);
}

int main()
{
    setlocale(LC_CTYPE, "RU");
    vector<vector<int> > a;
    vector<int> b;
    vector<int> c;
    int size;

    cout << "Введите желаемую размерность матрицы/вектора: ";
    cin >> size;

    for (int i = 0; i < size; i++)
    {
        vector<int> tmp;
        a.push_back(tmp);
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            a[i].push_back(getRandomUpTo(RAND_MAX_VALUE));
        }
        b.push_back(getRandomUpTo(RAND_MAX_VALUE));
        c.push_back(0);
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            c[i] += a[i][j] * b[i];
        }
    }

    return 0;
}