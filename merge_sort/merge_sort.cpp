/*****************************************************************//**
 * @file   merge_sort.cpp
 * @brief  Parallel merge sort
 * using barrier 
 * @author Hikmat Farhat
 * @date   March 2021
 *********************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include <barrier>
#include <thread>
#include <random>

const int n = 1 << 25;
std::vector<int> v(n);
std::vector<int> u(n);
int const num_threads = 8;
std::barrier barrier(num_threads);

template <typename Iter>
void merge(Iter left, Iter right) {

	int d = std::distance(left, right);
	Iter end_left = left + d / 2;
	Iter end_right = right;
	Iter i = left;
	Iter j = end_left;

	std::vector<Iter::value_type> v(d);
	auto itr = v.begin();

	while ((i != end_left) && (j != end_right)) {
		if (*i < *j) {
			*itr = *i;
			++i; ++itr;
		}
		else {
			*itr = *j;
			++j; ++itr;
		}
	}
	if (i != end_left)
		std::copy(i, end_left, itr);
	if (j != end_right)
		std::copy(j, end_right, itr);

	std::copy(v.begin(), v.end(), left);
}
template<typename Iter>
void worker(Iter first, Iter second, Iter end, int idx) {
	auto d = std::distance(first, second);
	std::sort(first, second);
	idx /= 2;
	while ((idx % 2) == 0) {

		if (second == end)break;
		std::advance(second, d);
		d *= 2;
		idx /= 2;
		barrier.arrive_and_wait();
		merge<Iter>(first, second);

	}
	barrier.arrive_and_drop();
}

int main()
{
	std::random_device d;
	std::uniform_int_distribution dist(1, 100);
	std::generate_n(u.begin(), n, [&]() {return dist(d); });
	std::generate_n(v.begin(), n, [&]() {return dist(d); });
	std::vector<std::thread> mythreads;
	using vitr = std::vector<int>::iterator;

	vitr first = v.begin();
	int offset = v.size() / num_threads;
	vitr second = first;
	std::advance(second, offset);
	int count = 0;
	auto start = std::chrono::high_resolution_clock::now();
	while (true) {
		mythreads.push_back(std::thread(worker<vitr>, first, second, v.end(), count));
		count += 2;
		std::advance(first, offset);
		if (first == v.end())
			break;
		std::advance(second, offset);
	}

	std::cout << "number of threads " << mythreads.size() << "\n";
	for (auto& t : mythreads)
		t.join();
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration<double, std::milli>(end - start);
	std::cout << "parallel version time = " << duration.count() << std::endl;

	start = std::chrono::high_resolution_clock::now();
	std::sort(u.begin(), u.end());
	end = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration<double, std::milli>(end - start);
	std::cout << "sequential version time = " << duration.count() << std::endl;

	std::cout << std::boolalpha << std::is_sorted(v.begin(), v.end()) << "\n";

}

