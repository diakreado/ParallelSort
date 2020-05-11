#include <iostream>
#include <vector>
#include <omp.h>

#include <ctime>
#include <cstdlib>

#include <algorithm>
#include <iterator>
#include <forward_list>


enum class AlgoritmType_e {
	standard_library,
	sequential,
	parallel
};


constexpr int             g_cutoff      = 1000;
const     int             g_srand_init  = 4534534;
constexpr int             g_numThreads  = 4;
constexpr size_t          g_rand_size   = 10*1000*1000;
constexpr AlgoritmType_e  g_algoritm    = AlgoritmType_e::parallel;
constexpr int             g_gen_limit   = 100;



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
	if(first == last) { return; }
	auto pivot = *std::next(first, std::distance(first,last) / 2);
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

void quicksort_p(std::vector<int>& arr)
{
	#pragma omp parallel num_threads(g_numThreads)
	{
		#pragma omp master
		{
			__quicksort_p(arr.begin(), arr.end(), g_cutoff);
		}
	}
}


int main(int, char**)
{
	std::vector<int> arr;

	srand(g_srand_init);

	for (size_t i = 0; i < g_rand_size; ++i)
	{
		if constexpr (g_gen_limit == 0) { arr.push_back(rand()); }
		else                           { arr.push_back(rand() % g_gen_limit); }
	}

	double t1 = omp_get_wtime();

	if constexpr (g_algoritm == AlgoritmType_e::sequential)
	{
		quicksort(arr);
	}
	else if constexpr (g_algoritm == AlgoritmType_e::parallel)
	{
		quicksort_p(arr);
	}
	else if constexpr (g_algoritm == AlgoritmType_e::standard_library)
	{
		std::sort(arr.begin(), arr.end());
	}


	double t2 = omp_get_wtime();
	printf("%zu, %lf, %d\n", g_rand_size, t2-t1, g_numThreads);

	return 0;
}
