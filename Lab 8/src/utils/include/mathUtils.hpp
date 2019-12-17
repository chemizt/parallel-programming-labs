#include <vector>

#include "auxUtils.hpp"
#include "typeDefs.hpp"

#define TEST_ENTITY_SIZE 10
#define WORKSET_SIZE_OFFSET 3 // сдвиг показателя степени 10 (т.к. проводится 4 прогона с размерностями от 10^3 до 10^6)
#define WORKSET_STEP_COUNT 3
#define THREAD_STEP_COUNT 5

int calculateVectorSum(vector<int> vector);
