#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <locale>

#include "auxUtils.hpp"
#include "typeDefs.hpp"

using namespace std::chrono;
using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::locale;
using std::localtime;
using std::ofstream;
using std::put_time;
using std::setprecision;
using std::time_t;
using std::vector;

#define LOG_FILE_NAME "log.txt"

void outputIntMatrix(intMatrix matrix);

void outputIntVector(vector<int> vector);

void outputDoubleMatrix(doubleMatrix matrix);

void outputDoubleVector(vector<double> vector);

void logOutput(string message);

template <typename type>
void outputMatrixToFile(vector<vector<type>> matrix, string fileName);

template <typename type>
void outputVectorToFile(vector<type> vector, string fileName);
