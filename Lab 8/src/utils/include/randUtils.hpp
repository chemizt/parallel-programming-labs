#include <random>
#include <vector>

#include "typeDefs.hpp"

#define RAND_MAX_VALUE 10000

using std::mt19937_64;
using std::random_device;
using std::uniform_int_distribution;
using std::vector;

int getRandomUpTo(int maxValue);

vector<int> createRandFilledVector(uInt size);

intMatrix createRandFilledSquareMatrix(uInt size);

