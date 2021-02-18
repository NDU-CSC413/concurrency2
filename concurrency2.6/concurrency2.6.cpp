/*****************************************************************//**
 * @file   concurrency2.6.cpp
 * @brief  condition variables
 * TODO  condition variables
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

#include<atomic>
#include <thread>
#include <iostream>
#include <condition_variable>
std::mutex m;
std::condition_variable condVar;

bool ready{ false };

void writeT() {
	std::unique_lock<std::mutex> lck(m);

}
void readT() {
	std::unique_lock<std::mutex> lck(m);
	condVar.wait(lck, []() {return ready; });
}
int main() {


}

