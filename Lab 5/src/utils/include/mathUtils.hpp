#include <vector>

#include "auxUtils.hpp"
#include "typeDefs.hpp"

#define TESTSET_SIZE 4
#define WORKSET_BASE_SIZE 5
#define WORKSET_SIZE_STEP 1
#define WORKSET_STEP_COUNT 2
#define THREAD_STEP_COUNT 7

vector<int> multiplyMatrixAndVector(intMatrix matrix, vector<int> vector);

uInt calculateWorksetSize(uInt multiplier);

uInt calculateMaxWorksetSize();

uInt min(uInt x, uInt y);

uInt max(uInt x, uInt y);
