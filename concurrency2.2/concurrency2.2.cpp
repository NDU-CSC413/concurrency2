/*****************************************************************//**
 * @file   concurrency2.2.cpp
 * @brief  Deadlock using lock_guard
 * lock_guard does not solve all deadlock situations
 * Classical producer/consumer with bounded buffer
 * In this case locking two semaphores in wrong order
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>

using m = std::chrono::milliseconds;
#define NUM 20
const int capacity = 2<<20;
int size = 0;
int array[capacity]{};

std::mutex a, s;
std::random_device rd;
std::uniform_int_distribution<> dist(1, 5);
void producer() {
    int value = 9;
    
    for (int i = 0; i < NUM; ++i) {
        std::this_thread::sleep_for(m(dist(rd)));

        std::lock_guard gs(s);
        std::lock_guard ga(a);
        if (size == capacity)continue;
        std::cout << "\r" <<i;
        std::cout.flush();
        array[size++] = value;
    }
}
void consumer() {
    int val;
    int counter = 0;
    for (int i = 0; i < NUM; ++i) {
        std::this_thread::sleep_for(m(dist(rd)) * 10);
        /* wrong order of locking*/
        std::lock_guard ga(a);
        std::lock_guard gs(s);

        if (size != 0) {
         val = array[size--];
         std::cout << "\t" << ++counter;
        }
    }
}
int main()
{
    std::thread p(producer);
    std::thread c(consumer);
    std::cout << "#Writes\t #Reads\n";
    p.join();
    c.join();


}

