#include <vector>
#include <string>
#include <math.h>

#include "typeDefs.hpp"

using std::string;
using std::vector;

string getSpaces(uInt amount);

uInt getNumberLength(int number);

uInt getMaxNumberLengthInMatrix(intMatrix matrix);

vector<int> createInitializedVector(uInt size);

vector<double> calculateAverageAccelFromDoubleMatrix(doubleMatrix matrix);
