#include <iostream>
#include <omp.h>

using std::cout;
using std::endl;

int main()
{
    cout << "Max threads : " << omp_get_max_threads() << "\n";
    omp_set_num_threads(10);

#pragma omp parallel
    {
        cout << "Hello, World! Thread #" << omp_get_thread_num() << endl;
    }

    return 0;
}