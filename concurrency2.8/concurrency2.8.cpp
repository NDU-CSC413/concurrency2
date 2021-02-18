/*****************************************************************//**
 * @file   concurrency2.8.cpp
 * @brief  async,futures and promises
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

#include <iostream>
#include <thread>
#include <future>
#include <chrono>

using namespace std::literals::chrono_literals;

#define USELESS
#ifdef USELESS
std::future<int>  useless(int val) {
	std::promise<int> p;
	p.set_value(2 * val);
	return p.get_future();
}
int main() {
	std::future<int> fut = useless(8);
	std::cout << fut.get() << "\n";
}
#endif // USELESS

#ifdef THREAD

void threadf(std::promise<int> p) {
	std::this_thread::sleep_for(5s);
	p.set_value(19);
}
int main() {
	std::promise<int> prom;
	std::future<int> fut = prom.get_future();
	std::thread t(threadf, std::move(prom));
	std::cout << "waiting for result\n";
	std::cout << fut.get() << "\n";
	t.join(); 
}
#endif // THREAD

#ifdef ASYNC


int asyncf(int val) {
	std::this_thread::sleep_for(5s);
	return 2 * val;
}
int main()
{
	std::future<int> fut = std::async(asyncf, 23);
	std::cout << fut.get() << "\n";

}
#endif // ASYNC
