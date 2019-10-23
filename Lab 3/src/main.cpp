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
using namespace std::chrono;

#define TEST_ENTITY_SIZE 10
#define WORKING_ENTITY_SIZE_OFFSET 3 // сдвиг показателя степени 10 (т.к. проводится 4 прогона с размерностями от 10^3 до 10^6)
#define REDUCTION_RESULTS_FILENAME "redResults.csv"
#define CRITSECT_RESULTS_FILENAME "critSectResults.csv"

int main()
{
    setlocale(LC_CTYPE, "RU");
    intMatrix redDurMatrix;
    intMatrix critDurMatrix;
    doubleMatrix redAccelMatrix;
    doubleMatrix critAccelMatrix;
    vector<int> firstBaseVector;
    vector<int> secondBaseVector;
    uInt versatileVar;
    int result;

    cout << "Список операций:\n\n"
    << "0. Выход\n"
    << "1. Тестовый прогон\n"
    << "2. Рабочие прогоны (параллельный цикл, редукция)\n"
    << "3. Рабочие прогоны (критические секции)\n"
    << "4. Просмотр результатов (параллельный цикл, редукция)\n"
    << "5. Просмотр результатов (критические секции)\n\n";
    
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
                result = 0;
                
                cout << "Тестирование проводится на векторах "
                << "размерности 1x" << TEST_ENTITY_SIZE << "\n";

                cout << "\nВведите содержимое первого вектора "
                << "(целые числа; вводятся через пробел):\n";
                firstBaseVector = createHandFilledVector(TEST_ENTITY_SIZE);

                cout << "\nВведите содержимое второго вектора "
                << "(целые числа; вводятся через пробел):\n";
                secondBaseVector = createHandFilledVector(TEST_ENTITY_SIZE);

                cout << "\nИсходный вектор №1:\n";
                outputIntVector(firstBaseVector);

                cout << "Исходный вектор №2:\n";
                outputIntVector(secondBaseVector);

                result = calculateVectorSum(firstBaseVector) + calculateVectorSum(secondBaseVector);

                cout << "Результат: " << result << "\n\n";
                break;
            }
            case 2:
            {
                redDurMatrix.clear();            
                redAccelMatrix.clear();

                for (uInt t = 0; t <= 5; t++)
                {
                    uInt threadsQuantity = static_cast<uInt>(round(pow(2, t)));
                    vector<int> tmpDurVector;
                    vector<double> tmpAccelVector;

                    cout << "Количество потоков: " << threadsQuantity << "\n\n";

                    for (uInt p = 0; p <= 3; p++)
                    {
                        firstBaseVector.clear();
                        secondBaseVector.clear();

                        uInt size = static_cast<uInt>(round(pow(10, p + WORKING_ENTITY_SIZE_OFFSET)));
                        result = 0;

                        firstBaseVector = createInitializedVector(size);
                        secondBaseVector = createInitializedVector(size);

                        cout << "Прогон сложения элементов векторов размерности 1x"
                        << size << endl;

                        auto start = steady_clock::now();
                        #pragma omp parallel for
                        for (uInt i = 0; i < size; i++)
                        {
                            firstBaseVector.at(i) = getRandomUpTo(RAND_MAX_VALUE);
                            secondBaseVector.at(i) = getRandomUpTo(RAND_MAX_VALUE);
                        }
                        auto prepDur = duration_cast<milliseconds>(steady_clock::now() - start);

                        cout << "Подготовка данных заняла " << prepDur.count() << " мс" << endl;

                        auto startMicros = steady_clock::now();
                        #pragma omp parallel for num_threads(threadsQuantity) reduction(+:result)
                        for (uInt i = 0; i < size; i++)
                        {
                            result += firstBaseVector.at(i) + secondBaseVector.at(i); 
                        }
                        auto sumDur = duration_cast<microseconds>(steady_clock::now() - startMicros);

                        cout << "Сложение заняло " << sumDur.count() << " мкс\n" << endl;

                        tmpDurVector.push_back(sumDur.count());
                        if (threadsQuantity == 1)
                        {
                            tmpAccelVector.push_back(static_cast<double>(1));
                        }
                        else
                        {
                            tmpAccelVector.push_back(static_cast<double>(redDurMatrix.at(0).at(p)) / static_cast<double>(sumDur.count()));
                        }
                    }
                    redDurMatrix.push_back(tmpDurVector);
                    redAccelMatrix.push_back(tmpAccelVector);
                }
                break;
            }
            case 3:
            {
                critDurMatrix.clear();            
                critAccelMatrix.clear();

                for (uInt t = 0; t <= 5; t++)
                {
                    uInt threadsQuantity = static_cast<uInt>(round(pow(2, t)));
                    vector<int> tmpDurVector;
                    vector<double> tmpAccelVector;

                    cout << "Количество потоков: " << threadsQuantity << "\n\n";

                    for (uInt p = 0; p <= 3; p++)
                    {
                        firstBaseVector.clear();
                        secondBaseVector.clear();

                        uInt size = static_cast<uInt>(round(pow(10, p + WORKING_ENTITY_SIZE_OFFSET)));
                        result = 0;

                        firstBaseVector = createInitializedVector(size);
                        secondBaseVector = createInitializedVector(size);

                        cout << "Прогон сложения элементов векторов размерности 1x"
                        << size << endl;

                        auto start = steady_clock::now();
                        #pragma omp parallel for
                        for (uInt i = 0; i < size; i++)
                        {
                            firstBaseVector.at(i) = getRandomUpTo(RAND_MAX_VALUE);
                            secondBaseVector.at(i) = getRandomUpTo(RAND_MAX_VALUE);
                        }
                        auto prepDur = duration_cast<milliseconds>(steady_clock::now() - start);

                        cout << "Подготовка данных заняла " << prepDur.count() << " мс" << endl;

                        auto startMicros = steady_clock::now();
                        #pragma omp parallel for num_threads(threadsQuantity)
                        for (uInt i = 0; i < size; i++)
                        {
                            #pragma omp critical
                            {
                                result += firstBaseVector.at(i) + secondBaseVector.at(i);
                            }
                        }
                        auto sumDur = duration_cast<microseconds>(steady_clock::now() - startMicros);

                        cout << "Сложение заняло " << sumDur.count() << " мкс\n" << endl;

                        tmpDurVector.push_back(sumDur.count());
                        if (threadsQuantity == 1)
                        {
                            tmpAccelVector.push_back(static_cast<double>(1));
                        }
                        else
                        {
                            tmpAccelVector.push_back(static_cast<double>(critDurMatrix.at(0).at(p)) / static_cast<double>(sumDur.count()));
                        }
                    }
                    critDurMatrix.push_back(tmpDurVector);
                    critAccelMatrix.push_back(tmpAccelVector);
                }
                break;
            }
            case 4:
            {
                if (redDurMatrix.size() == 0)
                {
                    cout << "Для просмотра результатов необходимо сначала выполнить рабочие прогоны с использованием редукции\n\n";
                }
                else
                {
                    cout << "Элементами следующей матрицы являются значения времени выполнения умножения; "
                    << "по горизонтали они расположены в порядке возрастания объёма суммируемых векторов (10^3-10^6), "
                    << "а по вертикали - в порядке возрастания числа потоков (степени двойки от 1 до 32 включительно). "
                    << "Время приведено в микросекундах.\n";

                    outputIntMatrix(redDurMatrix);

                    cout << "Элементами следующей матрицы являются коэффициенты ускорения операции сложения "
                    << "для каждого размера векторов относительно её однопоточной реализации; "
                    << "время выполнения в однопоточной реализации взято за 1.\n";

                    outputDoubleMatrix(redAccelMatrix);

                    cout << "Средние значения ускорения для каждого количества потоков:\n";

                    outputDoubleVector(calculateAverageAccelFromDoubleMatrix(redAccelMatrix));
                }
                break;
            }
            case 5:
            {
                if (critDurMatrix.size() == 0)
                {
                    cout << "Для просмотра результатов необходимо сначала выполнить рабочие прогоны с использованием критических секций\n\n";
                }
                else
                {
                    cout << "Элементами следующей матрицы являются значения времени выполнения сложения; "
                    << "по горизонтали они расположены в порядке возрастания объёма суммируемых векторов (10^3-10^6), "
                    << "а по вертикали - в порядке возрастания числа потоков (степени двойки от 1 до 32 включительно). "
                    << "Время приведено в микросекундах.\n";

                    outputIntMatrix(critDurMatrix);

                    cout << "Элементами следующей матрицы являются коэффициенты ускорения операции сложения "
                    << "для каждого размера векторов относительно её однопоточной реализации; "
                    << "время выполнения в однопоточной реализации взято за 1.\n";

                    outputDoubleMatrix(critAccelMatrix);

                    cout << "Средние значения ускорения для каждого количества потоков:\n";

                    outputDoubleVector(calculateAverageAccelFromDoubleMatrix(critAccelMatrix));
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