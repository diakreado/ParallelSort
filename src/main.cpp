#include <iostream>
#include <array>
#include <omp.h>


int main(int, char**)
{
	// #pragma omp parallel
	// std::cout << "Hello, world!" << std::endl;

	std::array<int, 3> a = {1,4,3};

	for (auto i : a) std::cout << i << std::endl;
}
