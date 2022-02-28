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
#define NUM 150
const int capacity = 1<<10;
int size = 0;
int array[capacity]{};

std::mutex a, s;// to protect array and size
std::random_device rd;
std::uniform_int_distribution<> dist(1, 10);
void producer() {
    int value = 9;
    int count=0,tmp;
    while(count<NUM ) {
       std::this_thread::sleep_for(m(dist(rd)));
        // when using pre c++17 add template parameter
        // e.g. std::lock_guard<std::mutex> gs(s);
          
		std::lock_guard gs(s);
		if (size == capacity)continue;
		tmp = size++;
        std::lock_guard ga(a);
        array[tmp] = value;
        count++;
        std::cout << "\r" <<count;
        std::cout.flush();
    }
}
void consumer() {
    int val;
    int count = 0;
   while (count< NUM) {
        std::this_thread::sleep_for(m(dist(rd)*2) );
        /* wrong order of locking*/
       
        std::lock_guard gs(s);
        std::lock_guard ga(a);
       
        if (size != 0) {
         val = array[size--];
         count++;
         std::cout << "\r\t" <<count;
         std::cout.flush();
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

