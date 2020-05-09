#include <iostream>
#include <vector>
#include <omp.h>

#include <ctime>
#include <cstdlib>


int partition (std::vector<int>& arr, int low, int high)
{
	int pivot = arr[high];
	int i = (low - 1);

	auto swap = [](int* a, int* b)
	{
		int t = *a;
		*a = *b;
		*b = t;
	};

	for (int j = low; j <= high- 1; j++)
	{
		if (arr[j] <= pivot) 
		{
			i++;
			swap(&arr[i], &arr[j]); 
		}
	}
	swap(&arr[i + 1], &arr[high]); 
	return (i + 1); 
}

void quick_sort(std::vector<int>& arr, int low, int high) 
{
	if (low < high) 
	{
		int pi = partition(arr, low, high); 

		quick_sort(arr, low, pi - 1); 
		quick_sort(arr, pi + 1, high); 
	}
}

int main(int, char**)
{
	// #pragma omp parallel
	// std::cout << "Hello, world!" << std::endl;

	constexpr size_t rand_size = 40;

	std::vector<int> a;

	srand(time(0));

	for (size_t i = 0; i < rand_size; ++i)
	{
		a.push_back(rand()% 100 + 1);
	}

	for (auto i : a) { std::cout << i << " "; }
	std::cout << std::endl;

	quick_sort(a, 0, a.size()-1);

	for (auto i : a) { std::cout << i << " "; }
	std::cout << std::endl;
}


