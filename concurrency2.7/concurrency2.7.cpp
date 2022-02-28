/*****************************************************************//**
 * @file   concurrency2.7.cpp
 * @brief  Barriers
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <condition_variable>
#include <vector>

int num_threads = 10;
using Duration = std::chrono::seconds;
#define  CPP20
#ifndef CPP20

std::mutex m;
std::condition_variable condVar;
int count = 0;

void threadf(int i,Duration d) {
    std::this_thread::sleep_for(d);
    std::cout << i << " started\n";
    std::unique_lock<std::mutex> lck(m);
    ++count;
    condVar.wait(lck, []() {return count == num_threads; });
    condVar.notify_one();
    std::cout << i << " ended\n";

}
int main()
{
    std::random_device rd;
    std::uniform_int_distribution<> dist(1, 5);
    std::vector<std::thread> mythreads;
    for (int i = 0; i < num_threads - 1; ++i)
        mythreads.push_back(
            std::thread(threadf, i, Duration(dist(rd)))
        );
    /* let them wait for the last thread */
    mythreads.push_back(
        std::thread(threadf, num_threads - 1, Duration(15))
    );
    for (auto& t:mythreads)
        t.join();
}
#else
#include <barrier>
std::barrier barrier{ num_threads };

void threadf(int i, Duration d) {
    std::this_thread::sleep_for(d);
    std::cout << i << " started\n";
    barrier.arrive_and_wait();
    std::cout << i << " ended\n";

}
int main()
{
    std::random_device rd;
    std::uniform_int_distribution<> dist(1, 5);
    std::vector<std::thread> mythreads;
    for (int i = 0; i < num_threads - 1; ++i)
        mythreads.push_back(
            std::thread(threadf, i, Duration(dist(rd)))
        );
    /* let them wait for the last thread */
    mythreads.push_back(
        std::thread(threadf, num_threads - 1, Duration(15))
    );
    for (auto& t : mythreads)
        t.join();
}
#endif // CPP20


