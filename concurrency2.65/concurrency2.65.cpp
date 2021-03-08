/*****************************************************************//**
 * @file   concurrency2.65.cpp
 * @brief  More detailed example
 *  of condition variables
 * @author Hikmat Farhat
 * @date   March 2021
 *********************************************************************/

#include <thread>
#include <iostream>
#include <ratio>
#include <chrono>
#include <condition_variable>

using clk = std::chrono::high_resolution_clock;
using namespace std::literals::chrono_literals;
std::mutex m;
std::condition_variable condVar;

bool ready{ false };

double currentT() {
	using Duration = std::chrono::duration<double, std::ratio<1, 1>>;
	static auto start = std::chrono::high_resolution_clock::now();

	return Duration(clk::now() - start).count();
}


void threadf() {
	std::this_thread::sleep_for(5s);

	std::unique_lock<std::mutex> lck(m);
	std::cout << "F started\t " << currentT() << "\n";
	std::this_thread::sleep_for(5s);
	condVar.notify_one();
	std::cout << "notification sent\t" << currentT() << "\n";
	lck.unlock();
	std::this_thread::sleep_for(1s);
	lck.lock();
	std::cout << "F is done\t" << currentT() << "\n";
}
void threadg() {
	std::unique_lock<std::mutex> lck(m);
	std::cout << "Waiting for F\n";
	currentT();//initialize clock
	condVar.wait(lck);
	std::cout << "notification received \t" << currentT() << "\n";
	std::this_thread::sleep_for(5s);
	std::cout << "G is done \t" << currentT() << "\n";

}
int main() {

	std::thread t1(threadf);
	std::thread t2(threadg);
	t1.join();
	t2.join();
}

