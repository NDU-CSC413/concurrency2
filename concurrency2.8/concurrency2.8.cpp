/*****************************************************************//**
 * @file   concurrency2.8.cpp
 * @brief  async,futures and promises
 * TODO: async
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

#include <iostream>
#include <thread>
#include <future>
#include <chrono>

using namespace std::literals::chrono_literals;
std::future<int>  useless(int val) {
	std::promise<int> p;
	p.set_value(2 * val);
	return p.get_future();
}
void threadf(std::promise<int> p) {
	std::this_thread::sleep_for(5s);
	p.set_value(19);
}
int asyncf(int val) {
	std::this_thread::sleep_for(5s);
	return 2 * val;
}
int main()
{
	
	/*std::future<int> fut = useless(8);
	std::cout << fut.get() << "\n";

	std::promise<int> prom;
	fut = prom.get_future();
	std::thread t(threadf,std::move(prom));
	std::cout << "waiting for result\n";
	fut.wait();
	std::cout << fut.get() << "\n";
	t.join();*/
	std::future<int> fut = std::async(asyncf, 23);
	fut.wait();
	std::cout << fut.get() << "\n";

}
