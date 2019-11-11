#include <iostream>
#include <fstream>
#include <locale>
#include <vector>
#include <chrono>
#include <iomanip>
#include <math.h>
#include <mpi.h>

#include "auxUtils.hpp"
#include "typeDefs.hpp"
#include "outUtils.hpp"
#include "inUtils.hpp"
#include "randUtils.hpp"
#include "mathUtils.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::ofstream;
using std::ios_base;
using std::locale;
using std::vector;
using std::put_time;
using namespace std::chrono;


#define OUTPUT_FILE_NAME "results.csv"

struct Comma final : std::numpunct<char> // inspired by (copy-typed from) https://stackoverflow.com/a/42331536
{
    char do_decimal_point() const override { return ','; }
};

template <typename type>
void outputMatrixToFile(vector<vector<type> > matrix) //TODO: move this and outputVectorToFile to outUtils
{
    ofstream resultsFile;
    resultsFile.open(OUTPUT_FILE_NAME, std::ios_base::app);
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
void outputVectorToFile(vector<type> vector)
{
    ofstream resultsFile;
    resultsFile.open(OUTPUT_FILE_NAME, std::ios_base::app);
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
    intMatrix baseMatrix;
    intMatrix durMatrix;
    doubleMatrix accelMatrix;
    vector<int> baseVector;
    vector<int> resultVector;
    uInt versatileVar;
    ofstream resultsFile;

    resultsFile.open(OUTPUT_FILE_NAME);
    resultsFile.clear();
    resultsFile.close();

    MPI_Init(NULL, NULL);

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
                << TESTSET_SIZE << "x" << TESTSET_SIZE
                << " и векторе " << TESTSET_SIZE << "x1\n";

                cout << "\nВведите содержимое матрицы "
                << "(целые числа; вводятся через пробел):\n";
                baseMatrix = createHandFilledSquareMatrix(TESTSET_SIZE);

                cout << "\nВведите содержимое вектора "
                << "(целые числа; вводятся через пробел):\n";
                baseVector = createHandFilledVector(TESTSET_SIZE);

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

                for (uInt t = 0; t <= THREAD_STEP_COUNT; t++)
                {
                    uInt processesQuantity = round(pow(2, t));
                    vector<int> tmpDurVector;
                    vector<double> tmpAccelVector;

                    cout << "Количество процессов: " << processesQuantity << "\n\n";

                    for (uInt p = 0; p <= WORKSET_STEP_COUNT; p++)
                    {
                        MPI_Status status;
                        MPI_Comm workerComm;
                        int intermediateRes;
                        uInt sentQuantity = 0;
                        uInt worksetSize = calculateWorksetSize(p);

                        baseMatrix.clear();
                        baseVector.clear();

                        cout << "Прогон перемножения матрицы " << worksetSize << "x" << worksetSize
                        << " и вектора " << worksetSize << "x1" << endl;

                        auto start = steady_clock::now();
                        for (uInt x = 0; x < worksetSize; x++)
                        {
                            baseMatrix.push_back(createInitializedVector(worksetSize));
                        }
                        baseVector = createInitializedVector(worksetSize);

                        #pragma omp parallel for collapse(2)
                        for (uInt x = 0; x < worksetSize; x++)
                        {
                            for (uInt y = 0; y < worksetSize; y++)
                            {
                                baseMatrix.at(x).at(y) = getRandomUpTo(RAND_MAX_VALUE);
                            }
                        }

                        #pragma omp parallel for
                        for (uInt x = 0; x < worksetSize; x++)
                        {
                            baseVector.at(x) = getRandomUpTo(RAND_MAX_VALUE);
                        }
                        resultVector = createInitializedVector(worksetSize);
                        auto prepDur = duration_cast<milliseconds>(steady_clock::now() - start);

                        cout << "Подготовка данных заняла " << prepDur.count() << " мс" << endl;

                        auto startMicros = steady_clock::now();
                        if (processesQuantity >= 2)
                        {
                            MPI_Comm_spawn("worker", MPI_ARGV_NULL, processesQuantity, MPI_INFO_NULL, 0, MPI_COMM_SELF, &workerComm, MPI_ERRCODES_IGNORE);
                            MPI_Bcast(&worksetSize, 1, MPI_INT, MPI_ROOT, workerComm);
                            MPI_Bcast(baseVector.data(), worksetSize, MPI_INT, MPI_ROOT, workerComm);
                            for (uInt i = 0; i < min(worksetSize, processesQuantity); i++)
                            {
                                MPI_Send(baseMatrix.at(i).data(), worksetSize, MPI_INT, i, i + 1, workerComm);
                                sentQuantity++;
                            }
                            for (uInt i = 0; i < worksetSize; i++)
                            {
                                MPI_Recv(&intermediateRes, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, workerComm, &status);
                                resultVector.at(status.MPI_TAG - 1) = intermediateRes;
                                if (sentQuantity < worksetSize)
                                {
                                    MPI_Send(baseMatrix.at(sentQuantity).data(), worksetSize, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, workerComm);
                                    sentQuantity++;
                                }
                                else
                                {
                                    MPI_Send(MPI_BOTTOM, 0, MPI_INT, status.MPI_SOURCE, 0, workerComm);
                                }
                            }
                            MPI_Comm_free(&workerComm);
                        }
                        else
                        {
                            resultVector = multiplyMatrixAndVector(baseMatrix, baseVector);
                        }
                        auto multDur = duration_cast<milliseconds>(steady_clock::now() - startMicros);

                        cout << "Перемножение заняло " << multDur.count() << " мс\n" << endl;

                        tmpDurVector.push_back(multDur.count());
                        if (processesQuantity == 1)
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
                    outputMatrixToFile(durMatrix);

                    cout << "Элементами следующей матрицы являются коэффициенты ускорения операции умножения "
                    << "для каждого размера массивов относительно её однопоточной реализации; "
                    << "время выполнения в однопоточной реализации взято за 1.\n";

                    outputDoubleMatrix(accelMatrix);
                    outputMatrixToFile(accelMatrix);

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
    
    MPI_Finalize();
    return 0;
}