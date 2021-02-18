/*****************************************************************//**
 * @file   concurrency2.6.cpp
 * @brief  Spinlock
 * 
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

#include<atomic>
#include <thread>
#include <iostream>
//class spin_lock {
//    std::atomic_flag f;
//    spin_lock() :f(ATOMIC_FLAG_INIT) {}
//    void lock() {
//        while (f.test_and_set());
//    }
//    void unlock() {
//        f.clear();
//    }
//};
int main()
{
    std::thread t;
   /* std::atomic_flag f;
    bool r = f.test_and_set();
    std::cout << std::boolalpha << r << "\n";
    r = f.test_and_set();
    std::cout << std::boolalpha << r << "\n";*/

}

