#include <iostream>
#include <vector>

#include "auxUtils.hpp"
#include "typeDefs.hpp"
#include "outUtils.hpp"
#include "inUtils.hpp"
#include "randUtils.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

#define TEST_ENTITY_SIZE 4
#define WORKING_ENTITY_BASE_SIZE 500

int main()
{
    setlocale(LC_CTYPE, "RU");
    intMatrix a;
    vector<int> b;
    vector<int> c;
    uInt versatileVar;

    cout << "Список операций:\n\n"
    << "0. Выход\n"
    << "1. Тестовый прогон\n"
    << "2. Рабочие прогоны\n\n";
    
    do
    {
        cout << "Выберите операцию: ";
        cin >> versatileVar;
        cout << "\n";

        switch (versatileVar)
        {
            case 0:
            {
                cout << "Завершение работы...\n";
                break;
            }
            case 1:
            {
                cout << "Тестирование проводится на матрице "
                << TEST_ENTITY_SIZE << "x" << TEST_ENTITY_SIZE
                << " и векторе " << TEST_ENTITY_SIZE << "x1\n";

                cout << "\nВведите содержимое матрицы "
                << "(целые числа; вводятся через пробел):\n";
                a = createHandFilledSquareMatrix(TEST_ENTITY_SIZE);

                cout << "\nВведите содержимое вектора "
                << "(целые числа; вводятся через пробел):\n";
                b = createHandFilledVector(TEST_ENTITY_SIZE);

                c = createInitializedVector(TEST_ENTITY_SIZE);

                cout << "\nИсходная матрица:\n";
                outputMatrix(a);

                cout << "Исходный вектор:\n";
                outputVector(b);

                for (uInt i = 0; i < TEST_ENTITY_SIZE; i++)
                {
                    for (uInt j = 0; j < TEST_ENTITY_SIZE; j++)
                    {
                        c.at(i) += a.at(i).at(j) * b.at(j);
                    }
                }

                cout << "Результирующий вектор:\n";
                outputVector(c);

                break;
            }
            default:
            {
                cout << "Операция с таким идентификатором не определена!\n\n";
                break;
            }
        }
    } 
    while (versatileVar != 0);
    
    

    
    return 0;
}