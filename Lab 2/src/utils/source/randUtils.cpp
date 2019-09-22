#include "utils/include/randUtils.hpp"

int getRandomUpTo(int maxValue)
{
    random_device rD;
    mt19937_64 rNG(rD());
    uniform_int_distribution<> distribution(0, maxValue);

    rNG.seed(rNG());

    return distribution(rNG);
}