/*****************************************************************//**
 * @file   concurrency2.6.cpp
 * @brief  condition variables
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

#include<atomic>
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
#define EX2
#ifdef EX1
void writeT() {
	std::unique_lock<std::mutex> lck(m);
	//ready = true;
	std::cout << "wrote data\n";
	condVar.notify_one();
	std::this_thread::sleep_for(5s);
	std::cout << "writer finished first time\n";
	std::this_thread::sleep_for(5s);
	std::cout << "setting ready=true and unlocking\n";
	ready = true;
	lck.unlock();
	condVar.notify_one();
}
void readT() {
	std::cout << "Waiting for data\n";

	std::unique_lock<std::mutex> lck(m);
	condVar.wait(lck, []() {return ready; });
	std::cout << "Data recieved \n";
}
int main() {
	std::thread t1(readT);
	std::this_thread::sleep_for(10s);
	std::thread t2(writeT);
	t1.join();
	t2.join();

}
#endif 

#ifdef EX2
void threadf() {
	std::this_thread::sleep_for(5s);

	std::unique_lock<std::mutex> lck(m);
	std::cout << "F started\t " << currentT() << "\n";
	std::this_thread::sleep_for(5s);
	condVar.notify_one();
	std::cout << "notification sent\t"<<currentT() << "\n";
	lck.unlock();
	std::this_thread::sleep_for(1s);
	lck.lock();
	std::cout << "F is done\t"<<currentT()<< "\n";
}
void threadg() {
	std::unique_lock<std::mutex> lck(m);
	std::cout << "Waiting for F\n";
	currentT();//initialize clock
	condVar.wait(lck);
	std::cout << "notification received \t"<<currentT()<<"\n";
	std::this_thread::sleep_for(5s);
	std::cout <<"G is done \t" << currentT() << "\n";

}
int main() {

	std::thread t1(threadf);
	std::thread t2(threadg);
	t1.join();
	t2.join();
}
#endif // EX2

