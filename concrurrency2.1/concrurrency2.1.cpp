/*****************************************************************//**
 * @file   concrurrency2.1.cpp
 * @brief  Using lock_guard
 * 
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/
#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include  <string>


int y = 0;
int num_iterations = 100000;
int num_trials = 10; 


std::random_device rd;
std::uniform_int_distribution<> dist(50, 500);

std::mutex m;
int add(const std::vector<std::string>& values) {
    int val = 0;
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rd)));
    /* if your compiler supports c++17
    * std::lock_guard g(m);
    * is sufficient
    */
   std::lock_guard<std::mutex> g(m);
    for (int i = 0; i < values.size(); ++i)
        val += std::stoi(values[i]);
    return val;
}
int sub(const std::vector<std::string>& values) {
    int val = 0;
    /* wait twice as much as add on average */
    std::this_thread::sleep_for(std::chrono::milliseconds(2 * dist(rd)));
    std::lock_guard<std::mutex> g(m);
    for (int i = 0; i < values.size(); ++i)
        val -= std::stoi(values[i]);
    return val;
}


template <typename T>
void threadf(int& val, const T& values
    , int (*f)(const T&)) {
    /* we MUST catch exception here.
    * it cannot be caught by main
    */
    try {
        val += f(values);
    }
    catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }
}

int main()
{
    using V = std::vector<std::string>;
    int sum = 0;
    V values{ "1","2","3","4","5" };
    V evalues{ "p","2","3","4","5" };//causes exception in std::stoi
    /* due to exception this thread will exit without releasing mutex*/
    std::thread t1(threadf<V>, std::ref(sum), std::ref(evalues), add);
    /* this one will most probably hang waiting for the mutex to be released*/
    std::thread t2(threadf<V>, std::ref(sum), std::ref(values), sub);
    t1.join();
    t2.join();

    std::cout << sum << "\n";
}
