/*****************************************************************//**
 * @file   concurrency2.3.cpp
 * @brief  Readers-Writers using std::shared_mutex
 * 
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include <shared_mutex>

std::random_device e;
std::uniform_int_distribution<> dist(1, 50);
std::vector<int> v;// shared buffer

std::shared_mutex wrt;

class Reader {
public:
    void operator() () {
        int sum = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(e) * 100));        
        wrt.lock_shared();
        std::cout << "Reader thread " << std::this_thread::get_id() << " started\n";
        for (auto x : v) {
            sum += x;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << " Reader thread " << std::this_thread::get_id() << " ended\n";
        wrt.unlock_shared();
        if (sum != 0) std::cout << "sum in thread " << std::this_thread::get_id() << " is " << sum << std::endl;
    }
};

class Writer {

public:
    Writer() {
    }
    void operator() () {
        int value = dist(e);
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(e) * 100));
        wrt.lock();
        std::cout << " Writer thread " << std::this_thread::get_id() << " started\n";

        for (auto& x : v) {
            x = value;
            value = -value;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << " Writer thread " << std::this_thread::get_id() << " ended\n";
        wrt.unlock();

    }
};

int main()
{
    std::cout << "Concurrency 2.3 \n";
    std::cout << "------------------------------\n";
    int value = 1;
    for (int i = 0; i < 100; i++) {
        v.push_back(value);
        value = -value;
    }
    for (int i = 0; i < 4; i++) {
        std::cout << "Trial " << i << std::endl;
        std::vector<std::thread> mythreads;
        for (int j = 0; j < 2; j++) {
            Reader r1, r2;
            Writer w;
            std::thread t1(r1);
            std::thread t2(w);
            std::thread t3(r2);
            mythreads.push_back(std::move(t1));
            mythreads.push_back(std::move(t2));
            mythreads.push_back(std::move(t3));
        }
       
        for (auto& t : mythreads)
            t.join();
        std::cout << "----------------" << std::endl;
        
    }

}
