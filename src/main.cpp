#include <iostream>
#include <vector>
#include <omp.h>

#include <ctime>
#include <cstdlib>

#include <algorithm>
#include <iterator>
#include <forward_list>



template <class ForwardIt>
void __quicksort(ForwardIt first, ForwardIt last)
{
	if(first == last) return;
	auto pivot = *std::next(first, std::distance(first,last)/2);
	ForwardIt middle1 = std::partition(first, last,
							[pivot](const auto& em){ return em < pivot; });
	ForwardIt middle2 = std::partition(middle1, last,
						[pivot](const auto& em){ return !(pivot < em); });
	__quicksort(first, middle1);
	__quicksort(middle2, last);
}

void quicksort(std::vector<int>& arr)
{
	__quicksort(arr.begin(), arr.end());
}

template <class ForwardIt>
void __quicksort_p(ForwardIt first, ForwardIt last, int cutoff)
{
	if(first == last) return;
	auto pivot = *std::next(first, std::distance(first,last)/2);
	ForwardIt middle1 = std::partition(first, last,
							[pivot](const auto& em){ return em < pivot; });
	ForwardIt middle2 = std::partition(middle1, last,
						[pivot](const auto& em){ return !(pivot < em); });

	if (last - first < cutoff)
	{
		__quicksort_p(first, middle1, cutoff);
		__quicksort_p(middle2, last, cutoff);
	}
	else
	{
		#pragma omp task
		{ __quicksort_p(first, middle1, cutoff); }

		#pragma omp task
		{ __quicksort_p(middle2, last, cutoff); }
	}
}

void quick_sort_p(std::vector<int>& arr)
{

	constexpr int cutoff = 1000;
	constexpr int numThreads = 2;

	#pragma omp parallel num_threads(numThreads)
	{
		#pragma omp single nowait
		{
			__quicksort_p(arr.begin(), arr.end(), cutoff);
		}
	}
}


int main(int, char**)
{
	// #pragma omp parallel
	// std::cout << "Hello, world!" << std::endl;

	constexpr size_t rand_size = 1000*1000*100;

	std::vector<int> a;
	// std::vector<int> b;

	srand(time(0));

	for (size_t i = 0; i < rand_size; ++i)
	{
		int x = rand()% 100 + 1;
		a.push_back(x);
	}

	// get start time
	double t1 = omp_get_wtime();

	// for (auto i : a) { std::cout << i << " "; }
	// std::cout << std::endl;


	// quicksort(a);
	quick_sort_p(a);


	double t2 = omp_get_wtime();
	printf("%zu,%lf,%d\n", rand_size, t2-t1, omp_get_max_threads());

	// for (auto i : a) { std::cout << i << " "; }
	// std::cout << std::endl;
}


