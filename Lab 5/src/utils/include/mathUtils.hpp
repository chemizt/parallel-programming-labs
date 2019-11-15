#include <vector>

#include "auxUtils.hpp"
#include "typeDefs.hpp"

#define TESTSET_SIZE 4
#define WORKSET_BASE_SIZE 500
#define WORKSET_SIZE_STEP 100
#define WORKSET_STEP_COUNT 5
#define THREAD_STEP_COUNT 5

vector<int> multiplyMatrixAndVector(intMatrix matrix, vector<int> vector);

uInt calculateWorksetSize(uInt multiplier);

uInt calculateMaxWorksetSize();

uInt min(uInt x, uInt y);

uInt max(uInt x, uInt y);
