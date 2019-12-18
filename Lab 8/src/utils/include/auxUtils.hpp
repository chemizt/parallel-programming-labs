#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

#include "typeDefs.hpp"

using std::string;
using std::ostringstream;
using std::vector;
using std::fixed;
using std::setprecision;
using std::put_time;

string getSpaces(uInt amount);

uInt getIntNumberLength(int number);

uInt getDoubleNumberLength(double number);

uInt getMaxNumberLengthInIntMatrix(intMatrix matrix);

uInt getMaxNumberLengthInDoubleMatrix(doubleMatrix matrix);

vector<int> createInitializedVector(uInt size);

vector<double> calculateAverageAccelFromDoubleMatrix(doubleMatrix matrix);

string getCurrentTimeStampAsString();

void mSecSleep(unsigned long ms);