// radix-sort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/**
* implementation of parallel radix sort 
*/
// radix-sort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <numeric>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <thread>
#include <barrier>
#include <execution>
int total = 0;
using Duration = std::chrono::duration<double, std::milli>;
int num_threads = 8;
std::barrier barrier{ num_threads };

#define TIMEIT(dur,...)\
   {\
    auto start = std::chrono::high_resolution_clock::now();\
    __VA_ARGS__\
    auto end = std::chrono::high_resolution_clock::now();\
     dur = std::chrono::duration<double, std::milli>(end - start);\
}
void one_pass(int tid, std::vector<unsigned int>& a, std::vector<unsigned int>& b,
    std::vector<unsigned int>& t, std::vector<unsigned int>& f,
    int start, int end, int sb) {

    int size = a.size();

    for (int i = start; i < end; i++) {
        t[i] = (a[i] >> sb) & 1;
        t[i] = (~t[i]) & 1;

    }


    barrier.arrive_and_wait();
    if (tid == 0) {
        std::exclusive_scan(std::execution::par, t.begin(), t.end(), f.begin(), 0);
        total = t[size - 1] + f[size - 1];
        int abc = 33;

    }
    barrier.arrive_and_wait();


    for (int i = start; i < end; ++i) {

        //t[i] = ((a[i] >> sb) & 1) ? i - f[i] + total : f[i];
        if (((a[i] >> sb) & 1) == 1) {
            t[i] = i - f[i] + total;
        }
        else {
            t[i] = f[i];
        }
    }
    barrier.arrive_and_wait();
    /* for (auto x : t)
         std::cout << x << ",";
     std::cout << "\n";*/
    for (int i = start; i < end; ++i)
        b[t[i]] = a[i];
    barrier.arrive_and_wait();

}
void worker(int tid, std::vector<unsigned int>& a, std::vector<unsigned int>& b,
    std::vector<unsigned int>& t, std::vector<unsigned int>& f,
    int start, int end, int nbits) {
    int i = 0;
    for (; i < nbits + 1; ++i) {
        if (i % 2 == 0)
            one_pass(tid, a, b, t, f, start, end, i);
        else
            one_pass(tid, b, a, t, f, start, end, i);
    }
}
int main()
{
    std::random_device rd;
    std::uniform_int_distribution<unsigned int> dist(1, 100);
    const int n = 1 << 25;
    Duration d;
    double total_time = 0;
    int num_trials = 20;

    int chunk_size = n / num_threads;
    std::vector<unsigned int> data(n);
    std::vector<unsigned int> a(n);
    std::vector<unsigned int> c(n);
    std::vector<unsigned int> b(n);
    std::vector<unsigned int> t(n);
    std::vector<unsigned int> f(n);


    std::generate(data.begin(), data.end(), [&]() {
        return dist(rd);
        });
    std::cout << "Done generation\n";
    /* for (auto x : data)
         std::cout << x << ",";
     std::cout << "\n";*/
    for (int k = 0; k < num_trials; ++k) {
        a = data;
        //std::vector<unsigned int> a{ 4,7,2,6,3,5,1,0 };
        int max = *std::max_element(a.begin(), a.end());
        int nbits = static_cast<int> (std::floor(std::log2(max)) + 1);

        std::vector<std::thread> mythreads;
        TIMEIT(d,
            for (int i = 0; i < num_threads; ++i) {
                int start = i * chunk_size;
                int end = (i + 1) * chunk_size;
                mythreads.push_back(
                    std::thread(
                        worker, i, std::ref(a), std::ref(b), std::ref(t), std::ref(f)
                        , start, end, nbits)
                );

            }
        for (auto& t : mythreads)
            t.join();
        )
            total_time += d.count();
    }
    std::cout << "radix sort time = " << total_time / num_trials << "\n";
    total_time = 0;
    for (int k = 0; k < num_trials; ++k) {
        c = data;

        TIMEIT(d,
            std::sort(std::execution::seq, c.begin(), c.end());
        )
            total_time += d.count();
    }
    std::cout << "std::sort(quicksort) time = " << total_time / num_trials << "\n";
    /*    for (auto x : b)
            std::cout << x << ",";
        std::cout << "\n";*/
    std::cout << std::is_sorted(a.begin(), a.end()) << "\n";
    // std::cout << std::is_sorted(b.begin(), b.end()) << "\n";


}
