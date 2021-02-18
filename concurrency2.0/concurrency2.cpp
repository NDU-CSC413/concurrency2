/*****************************************************************//**
 * \file   concurrency2.cpp
 * \brief   Illustration of race conditions
 * 
 * #define RACE using variables that are shared between multiple threads.
 * #define MUTEX  shows how using a mutex can fix the problem. 
 * #definte DEADLOCK shows how misshandled mutex can cause problems.
 * 
 * \author hikmat
 * \date   February 2021
 *********************************************************************/
/*
*/
#include <iostream>
#include <thread>
#include <exception>
#include <vector>
#include <mutex>

/* choose one of the defines below: RACE,MUTEX,DEADLOCK */

//#define RACE  //illustrate race condtion
//#define MUTEX // solution to race condition using std::mutex
#define DEADLOCK //mutex can lead to deadlock



#define NUM_ITERATIONS 100000
#define NUM_TRIALS 6


#ifdef RACE
void add(int& val) {
    for (int i = 0; i < NUM_ITERATIONS; ++i)
        ++val;
}
void sub(int& val) {
    for (int i = 0; i < NUM_ITERATIONS; ++i)
        --val;
}
int y = 0;
int main()
{
    for (int j = 0; j < NUM_TRIALS; ++j) {

        std::vector<std::thread > mythreads;
        int x = 0;
        y = 0;

        std::thread t1(add,std::ref(x));
        std::thread t2(add, std::ref(y));
       
        mythreads.push_back(std::move(t1));
        mythreads.push_back(std::move(t2));
        mythreads.push_back(std::thread(sub, std::ref(x)));
        mythreads.push_back(std::thread(sub,std::ref(y)));
        for (auto& t : mythreads)
            t.join();

       std::cout << "trial " << j << ",x=" << x << ",y=" << y << std::endl;
    }
}
#endif //RACE

#ifdef MUTEX
std::mutex m;

int main()
{
    auto add = [](int& val) {
        m.lock();
        for (int i = 0; i < NUM_ITERATIONS; ++i) {
            ++val;
        }

        m.unlock();
    };
    auto sub = [](int& val) {
        m.lock();
        for (int i = 0; i < NUM_ITERATIONS; ++i) {
            --val;
        }
        m.unlock();
    };
    for (int j = 0; j < NUM_TRIALS; j++) {

        std::vector<std::thread > mythreads;
        int x = 0;
        y = 0;
       
        std::thread t1(add, std::ref(x));
        std::thread t2(add, std::ref(y));
        mythreads.push_back(std::move(t1));
        mythreads.push_back(std::move(t2));

        mythreads.push_back(std::thread(sub, std::ref(x)));

        mythreads.push_back(std::thread(sub, std::ref(y)));
        for (auto& t : mythreads)
            t.join();

        std::cout << "trial " << j << ",x=" << x << ",y=" << y << std::endl;
    }
}
#endif // MUTEX


#ifdef DEADLOCK
#include  <string>
#include <random>
std::random_device rd;
std::uniform_int_distribution<> dist(50, 500);

std::mutex m;
int add(const std::vector<std::string>& values) {
    int val = 0;
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rd)));
    m.lock();
    for (int i = 0; i < values.size(); ++i)
        val += std::stoi(values[i]);
    m.unlock();
    return val;
}
int sub(const std::vector<std::string>& values) {
    int val = 0;
    /* wait twice as much as add on average */
    std::this_thread::sleep_for(std::chrono::milliseconds(2*dist(rd)));
    m.lock();
    for (int i = 0; i < values.size(); ++i)
        val -= std::stoi(values[i]);
    m.unlock();
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

#endif // DEADLOCK


