#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <math.h>
#include <omp.h>

#include "auxUtils.hpp"
#include "typeDefs.hpp"
#include "outUtils.hpp"
#include "inUtils.hpp"
#include "randUtils.hpp"
#include "mathUtils.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::put_time;
using namespace std::chrono;

#define TEST_ENTITY_SIZE 4
#define WORKING_ENTITY_BASE_SIZE 500
#define WORKING_ENTITY_SIZE_STEP 100

int main()
{
    setlocale(LC_CTYPE, "RU");
    intMatrix baseMatrix;
    vector<int> baseVector;
    vector<int> resultVector;
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
                baseMatrix = createHandFilledSquareMatrix(TEST_ENTITY_SIZE);

                cout << "\nВведите содержимое вектора "
                << "(целые числа; вводятся через пробел):\n";
                baseVector = createHandFilledVector(TEST_ENTITY_SIZE);

                cout << "\nИсходная матрица:\n";
                outputMatrix(baseMatrix);

                cout << "Исходный вектор:\n";
                outputVector(baseVector);

                resultVector = multiplyMatrixAndVector(baseMatrix, baseVector);

                cout << "Результирующий вектор:\n";
                outputVector(resultVector);
                break;
            }
            case 2:
            {
                for (uInt t = 0; t <= 5; t++)
                {
                    uInt threadsQuantity =  round(pow(2, t));

                    cout << "Количество потоков: " << threadsQuantity << "\n\n";

                    for (uInt p = 0; p <= 5; p++)
                    {
                        cout << "Прогон перемножения матрицы " << WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP << "x" << WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP
                             << " и вектора " << WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP << "x1" << endl;

                        baseMatrix = createRandFilledSquareMatrix(WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP);
                        baseVector = createRandFilledVector(WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP);
                        resultVector = createInitializedVector(WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP);

                        auto start = steady_clock::now();
                        #pragma omp parallel for num_threads(threadsQuantity) collapse(2)
                        for (uInt i = 0; i < WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP; i++)
                        {
                            for (uInt j = 0; j < WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP; j++)
                            {
                                resultVector.at(i) += baseMatrix.at(i).at(j) * baseVector.at(j);
                            }
                        }
                        auto end = steady_clock::now();

                        cout << "Перемножение заняло " << duration_cast<microseconds>(end - start).count() << " мс\n" << endl;
                    }
                }
                
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