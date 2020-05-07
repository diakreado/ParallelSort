#include <iostream>
#include <omp.h>


int main(int, char**)
{
    #pragma omp parallel
    std::cout << "Hello, world!\n";
}
