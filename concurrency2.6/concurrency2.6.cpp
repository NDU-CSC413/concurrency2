/*****************************************************************//**
 * @file   concurrency2.6.cpp
 * @brief  condition variables
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

#include<atomic>
#include <thread>
#include <iostream>
#include <chrono>
using namespace std::literals::chrono_literals;
#include <condition_variable>
std::mutex m;
std::condition_variable condVar;

bool ready{ false };

void writeT() {
	std::unique_lock<std::mutex> lck(m);
	ready = true;
	condVar.notify_one();

}
void readT() {
	std::this_thread::sleep_for(10s);
	std::unique_lock<std::mutex> lck(m);
	condVar.wait(lck, []() {return ready; });
}
int main() {
	std::thread t1(readT);
	std::this_thread::sleep_for(4s);
	std::thread t2(writeT);
	t1.join();
	t2.join();

}

