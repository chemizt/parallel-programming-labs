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
    intMatrix durMatrix;
    doubleMatrix accelMatrix;
    vector<int> baseVector;
    vector<int> resultVector;
    uInt versatileVar;

    cout << "Список операций:\n\n"
    << "0. Выход\n"
    << "1. Тестовый прогон\n"
    << "2. Рабочие прогоны\n"
    << "3. Просмотр результатов\n\n";
    
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
                outputIntMatrix(baseMatrix);

                cout << "Исходный вектор:\n";
                outputIntVector(baseVector);

                resultVector = multiplyMatrixAndVector(baseMatrix, baseVector);

                cout << "Результирующий вектор:\n";
                outputIntVector(resultVector);
                break;
            }
            case 2:
            {
                durMatrix.clear();            
                accelMatrix.clear();

                for (uInt t = 0; t <= 5; t++)
                {
                    uInt threadsQuantity = round(pow(2, t));
                    vector<int> tmpDurVector;
                    vector<double> tmpAccelVector;

                    cout << "Количество потоков: " << threadsQuantity << "\n\n";

                    for (uInt p = 0; p <= 5; p++)
                    {
                        baseMatrix.clear();
                        baseVector.clear();

                        cout << "Прогон перемножения матрицы " << WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP << "x" << WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP
                             << " и вектора " << WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP << "x1" << endl;

                        auto start = steady_clock::now();

                        for (uInt x = 0; x < WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP; x++)
                        {
                            baseMatrix.push_back(createInitializedVector(WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP));
                        }

                        #pragma omp parallel for collapse(2)
                        for (uInt x = 0; x < WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP; x++)
                        {
                            for (uInt y = 0; y < WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP; y++)
                            {
                                baseMatrix.at(x).at(y) = getRandomUpTo(RAND_MAX_VALUE);
                            }
                        }

                        #pragma omp for
                        for (uInt x = 0; x < WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP; x++)
                        {
                            baseVector.push_back(getRandomUpTo(RAND_MAX_VALUE));
                        }
                        resultVector = createInitializedVector(WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP);
                        auto prepDur = duration_cast<milliseconds>(steady_clock::now() - start);

                        cout << "Подготовка данных заняла " << prepDur.count() << " мс" << endl;

                        auto startMicros = steady_clock::now();
                        #pragma omp parallel for num_threads(threadsQuantity) collapse(2)
                        for (uInt i = 0; i < WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP; i++)
                        {
                            for (uInt j = 0; j < WORKING_ENTITY_BASE_SIZE + p * WORKING_ENTITY_SIZE_STEP; j++)
                            {
                                resultVector.at(i) += baseMatrix.at(i).at(j) * baseVector.at(j);
                            }
                        }
                        auto multDur = duration_cast<microseconds>(steady_clock::now() - startMicros);

                        cout << "Перемножение заняло " << multDur.count() << " мкс\n" << endl;

                        tmpDurVector.push_back(multDur.count());
                        if (threadsQuantity == 1)
                        {
                            tmpAccelVector.push_back(static_cast<double>(1));
                        }
                        else
                        {
                            tmpAccelVector.push_back(static_cast<double>(durMatrix.at(0).at(p)) / static_cast<double>(multDur.count()));
                        }
                    }
                    durMatrix.push_back(tmpDurVector);
                    accelMatrix.push_back(tmpAccelVector);
                }
                break;
            }
            case 3:
            {
                if (durMatrix.size() == 0)
                {
                    cout << "Для просмотра результатов необходимо сначала выполнить рабочие прогоны\n\n";
                }
                else
                {
                    cout << "Элементами следующей матрицы являются значения времени выполнения умножения; "
                    << "по горизонтали они расположены в порядке возрастания объёма перемножаемых массивов (500-1000), "
                    << "а по вертикали - в порядке возрастания числа потоков (степени двойки от 1 до 32 включительно). "
                    << "Время приведено в микросекундах.\n";

                    outputIntMatrix(durMatrix);

                    cout << "Элементами следующей матрицы являются коэффициенты ускорения операции умножения "
                    << "для каждого размера массивов относительно её однопоточной реализации; "
                    << "время выполнения в однопоточной реализации взято за 1.\n";

                    outputDoubleMatrix(accelMatrix);

                    cout << "Средние значения ускорения для каждого количества потоков:\n";

                    outputDoubleVector(calculateAverageAccelFromDoubleMatrix(accelMatrix));
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