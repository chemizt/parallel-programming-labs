#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <math.h>
#include <omp.h>
#include <vector>

#include "auxUtils.hpp"
#include "inUtils.hpp"
#include "mathUtils.hpp"
#include "outUtils.hpp"
#include "randUtils.hpp"
#include "typeDefs.hpp"

using namespace std::chrono;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::locale;
using std::ofstream;
using std::vector;

#define REDUCTION_RESULTS_FILENAME "redResults.csv"
#define CRITSECT_RESULTS_FILENAME "critSectResults.csv"

struct Comma final : std::numpunct<char> // inspired by (copy-typed from) https://stackoverflow.com/a/42331536
{
    char do_decimal_point() const override { return ','; }
};

template <typename type>
void outputMatrixToFile(vector<vector<type> > matrix, string fileName) //TODO: move this and outputVectorToFile to outUtils
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


int main()
{
    setlocale(LC_CTYPE, "RU");
    doubleMatrix critAccelMatrix;
    doubleMatrix redAccelMatrix;
    int result;
    intMatrix critDurMatrix;
    intMatrix redDurMatrix;
    uInt versatileVar;
    vector<int> firstBaseVector;
    vector<int> secondBaseVector;

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

                for (uInt t = 0; t <= THREAD_STEP_COUNT; t++)
                {
                    uInt processesQuantity = round(pow(2, t));
                    vector<int> tmpDurVector;
                    vector<double> tmpAccelVector;
                    microseconds multDur;

                    cout << "Количество процессов: " << processesQuantity << "\n\n";

                    for (uInt p = 0; p <= WORKSET_STEP_COUNT; p++)
                    {
                        uInt worksetSize = static_cast<uInt>(round(pow(10, p + WORKSET_SIZE_OFFSET)));
                        double execTime;
                        ifstream opDuration;

                        cout << "Прогон сложения элементов векторов размерности 1x"
                        << worksetSize << endl;
                        
                        if (processesQuantity >= 2)
                        {
                            system(("mpiexec -n " + std::to_string(processesQuantity + 1) + " workerRed " + std::to_string(worksetSize)).c_str());
                            opDuration.open("time");
                            opDuration >> execTime;
                            opDuration.close();
                            multDur = duration_cast<microseconds>(duration<double>(execTime));
                        }
                        else
                        {
                            int result;
                            
                            firstBaseVector.clear();
                            firstBaseVector.clear();
                            
                            firstBaseVector = createInitializedVector(worksetSize);
                            secondBaseVector = createInitializedVector(worksetSize);

                            #pragma omp parallel for
                            for (uInt i = 0; i < worksetSize; i++)
                            {
                                firstBaseVector.at(i) = getRandomUpTo(RAND_MAX_VALUE);
                                secondBaseVector.at(i) = getRandomUpTo(RAND_MAX_VALUE);
                            }
                    
                            auto startPoint = steady_clock::now();
                            result = calculateVectorSum(firstBaseVector) + calculateVectorSum(secondBaseVector);
                            multDur = duration_cast<microseconds>(steady_clock::now() - startPoint);
                        }

                        cout << "Сложение заняло " << multDur.count() << " мкс\n" << endl;

                        tmpDurVector.push_back(multDur.count());
                        if (processesQuantity == 1)
                        {
                            tmpAccelVector.push_back(static_cast<double>(1));
                        }
                        else
                        {
                            tmpAccelVector.push_back(static_cast<double>(redDurMatrix.at(0).at(p)) / static_cast<double>(multDur.count()));
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

                for (uInt t = 0; t <= THREAD_STEP_COUNT; t++)
                {
                    uInt processesQuantity = round(pow(2, t));
                    vector<int> tmpDurVector;
                    vector<double> tmpAccelVector;
                    microseconds multDur;

                    cout << "Количество процессов: " << processesQuantity << "\n\n";

                    for (uInt p = 0; p <= WORKSET_STEP_COUNT; p++)
                    {
                        uInt worksetSize = static_cast<uInt>(round(pow(10, p + WORKSET_SIZE_OFFSET)));
                        double execTime;
                        ifstream opDuration;

                        cout << "Прогон сложения элементов векторов размерности 1x"
                        << worksetSize << endl;
                        
                        if (processesQuantity >= 2)
                        {
                            system(("mpiexec -n " + std::to_string(processesQuantity + 1) + " workerCrit " + std::to_string(worksetSize)).c_str());
                            opDuration.open("time");
                            opDuration >> execTime;
                            opDuration.close();
                            multDur = duration_cast<microseconds>(duration<double>(execTime));
                        }
                        else
                        {
                            int result;
                            
                            firstBaseVector.clear();
                            firstBaseVector.clear();
                            
                            firstBaseVector = createInitializedVector(worksetSize);
                            secondBaseVector = createInitializedVector(worksetSize);

                            #pragma omp parallel for
                            for (uInt i = 0; i < worksetSize; i++)
                            {
                                firstBaseVector.at(i) = getRandomUpTo(RAND_MAX_VALUE);
                                secondBaseVector.at(i) = getRandomUpTo(RAND_MAX_VALUE);
                            }
                    
                            auto startPoint = steady_clock::now();
                            result = calculateVectorSum(firstBaseVector) + calculateVectorSum(secondBaseVector);
                            multDur = duration_cast<microseconds>(steady_clock::now() - startPoint);
                        }

                        cout << "Сложение заняло " << multDur.count() << " мкс\n" << endl;

                        tmpDurVector.push_back(multDur.count());
                        if (processesQuantity == 1)
                        {
                            tmpAccelVector.push_back(static_cast<double>(1));
                        }
                        else
                        {
                            tmpAccelVector.push_back(static_cast<double>(critDurMatrix.at(0).at(p)) / static_cast<double>(multDur.count()));
                        }
                    }
                    critDurMatrix.push_back(tmpDurVector);
                    critAccelMatrix.push_back(tmpAccelVector);
                }
                break;
            }
            case 4:
            {
                ofstream resultsFile;
                resultsFile.open(REDUCTION_RESULTS_FILENAME);
                resultsFile.clear();
                resultsFile.close();
                
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
                    outputMatrixToFile(redDurMatrix, REDUCTION_RESULTS_FILENAME);

                    cout << "Элементами следующей матрицы являются коэффициенты ускорения операции сложения "
                    << "для каждого размера векторов относительно её однопоточной реализации; "
                    << "время выполнения в однопоточной реализации взято за 1.\n";

                    outputDoubleMatrix(redAccelMatrix);
                    outputMatrixToFile(redAccelMatrix, REDUCTION_RESULTS_FILENAME);

                    cout << "Средние значения ускорения для каждого количества потоков:\n";

                    outputDoubleVector(calculateAverageAccelFromDoubleMatrix(redAccelMatrix));
                }
                break;
            }
            case 5:
            {
                ofstream resultsFile;
                resultsFile.open(CRITSECT_RESULTS_FILENAME);
                resultsFile.clear();
                resultsFile.close();
                
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
                    outputMatrixToFile(critDurMatrix, CRITSECT_RESULTS_FILENAME);

                    cout << "Элементами следующей матрицы являются коэффициенты ускорения операции сложения "
                    << "для каждого размера векторов относительно её однопоточной реализации; "
                    << "время выполнения в однопоточной реализации взято за 1.\n";

                    outputDoubleMatrix(critAccelMatrix);
                    outputMatrixToFile(critAccelMatrix, CRITSECT_RESULTS_FILENAME);

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