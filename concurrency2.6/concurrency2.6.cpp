/*****************************************************************//**
 * @file   concurrency2.6.cpp
 * @brief  simple example of condition variables
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
	{
		std::unique_lock<std::mutex> lck(m);
		ready = true;
		std::cout << "wrote data\n";
		condVar.notify_one();
	}
	std::this_thread::sleep_for(10s);
	std::cout << "writer finished\n";
}
void readT() {
	std::cout << "Waiting for data\n";

	std::unique_lock<std::mutex> lck(m);
	
	condVar.wait(lck, []() {return ready; });
	//--->
	/*
	* when we call wait
	* 1. wait unlocks lock
	* 2. blocks thread and adds it to list of waiting threads
	* 3. when notification is received
	* 4. wake up thread
	* 5. ->call lck.lock()
	*/
	std::cout << "Data received \n";
}
/*
* 1. waiting for data
* 2. wrote data
* 3. data received
* 4. writer finished
*/
int main() {
	std::thread t1(readT);
	std::this_thread::sleep_for(10s);
	std::thread t2(writeT);
	t1.join();
	t2.join();
}

