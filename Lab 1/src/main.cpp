#include <iostream>
#include <omp.h>

using std::cout;
using std::endl;

int main()
{
    #pragma omp parallel
    {
        cout << "Hello, World! Thread #" << omp_get_thread_num() << endl;
    }

    return 0;
}
