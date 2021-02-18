/*****************************************************************//**
 * @file   concurrency2.55.cpp
 * @brief  Using atomic
 * 
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

#include <iostream>
#include <thread>
#include <atomic>
#include <random>
#include <algorithm>
#include <cmath>

std::atomic<int>  y{ 0 };
int num_iterations = 100000;
int num_trials = 10;

int main()
{
   
    int a, b;
    std::default_random_engine e;
    std::uniform_int_distribution<> id(100000, 1000000);

    a = id(e);
    b = id(e);
    auto add = [a, b](std::atomic<int>& val) {
        for (int i = 0; i < num_iterations; ++i)
            ++val;
    };
    auto sub = [a, b](std::atomic<int>& val) {
        for (int i = 0; i < num_iterations; ++i) 
            --val;
    };
    for (int j = 0; j < num_trials; j++) {

        std::vector<std::thread > mythreads;
        std::atomic<int> x{ 0 };
        y = 0;
        //funcObject c(x, a, b, INC);
        std::thread t1(add, std::ref(x));
        std::thread t2(add, std::ref(y));
        /* below will not work because a
           vector will attempt to make a copy
           of t1 and t2 and the copy constructor
           for the thread class is deleted
        mythreads.push_back(t1);
        mythreads.push_back(t2);
        */
        mythreads.push_back(std::move(t1));
        mythreads.push_back(std::move(t2));

        mythreads.push_back(std::thread(sub, std::ref(x)));

        mythreads.push_back(std::thread(sub, std::ref(y)));
        for (auto& t : mythreads)
            t.join();

        std::cout << "trial " << j << ",x=" << x << ",y=" << y << std::endl;


    }

}
