/*****************************************************************//**
 * @file   concurrency2.4.cpp
 * @brief  Reader-Writers using shared_lock
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
#include <shared_mutex>
template<typename T>
using Duration = std::chrono::duration<double, T>;

#define TIMEIT(dur,...)\
   {\
    auto start = std::chrono::high_resolution_clock::now();\
    __VA_ARGS__\
    auto end = std::chrono::high_resolution_clock::now();\
     dur = std::chrono::duration<double>(end - start);\
}

std::random_device e;
std::uniform_int_distribution<> dist(50,75);
#define START 100
#define NUM_TRIALS 2
#define NUM_READERS 8
#define NUM_WRITERS 2
std::vector<int> v;

std::shared_mutex wrt;

class Reader {
public:
    void operator() () {
        int sum = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(e)*START));
        //std::lock_guard lock(wrt);
        std::shared_lock<std::shared_mutex> lock(wrt);
       
        std::cout << "Reader thread " << std::this_thread::get_id() << " started\n";
        for (auto x : v) {
            sum += x;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        std::cout << "Reader thread " << std::this_thread::get_id() << " ended\n";
        if (sum != 0) std::cout << "sum in thread " << std::this_thread::get_id() << " is " << sum << std::endl;
    }
};

class Writer {

public:
    Writer() {
    }
    void operator() () {
        int value = dist(e);
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(e)*START));
        /* Since we just need exclusive access 
        *  std::lock_guard l(wrt) also
        *  works but we keep unique_lock
        *  to make the meaning clear
        */
        std::unique_lock<std::shared_mutex> l(wrt);
        std::cout << "Writer thread " << std::this_thread::get_id() << " started\n";

        for (auto& x : v) {
            x = value;
            value = -value;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "Writer thread " << std::this_thread::get_id() << " ended\n";
    }
};

int main()
{
    std::cout << "Concurrency 2.4 \n";
    std::cout << "------------------------------\n";
    int value = 1;
    for (int i = 0; i < 100; i++) {
        v.push_back(value);
        value = -value;
    }
    Duration<std::ratio<1>> d;
    TIMEIT(d,
        for (int i = 0; i < NUM_TRIALS; ++i) {
            std::cout << "Trial " << i << std::endl;
            std::vector<std::thread> mythreads;
            for (int j = 0; j < NUM_READERS; ++j) 
                mythreads.push_back(std::thread{ Reader{} });
            for(int j=0;j<NUM_WRITERS;++j)
                mythreads.push_back(std::thread{ Writer{} });        
        
            for (auto& t : mythreads)
                t.join();
            std::cout << "----------------" << std::endl;
        }
    )
        std::cout << "total duration = " << d.count() << "\n";
}
