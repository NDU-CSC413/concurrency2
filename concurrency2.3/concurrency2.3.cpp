// readersWriters.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
#include <shared_mutex>

std::random_device e;
std::uniform_int_distribution<> dist(1, 50);
std::vector<int> v;
int nreaders = 0;
#define SYNC
#ifdef SYNC
std::shared_mutex wrt;
std::mutex m;
#endif // SYNC

class Reader {
public:
    static int num;
    void operator() () {
        int sum = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(e) * 100));

#ifdef SYNC
        
     wrt.lock_shared();
#endif 
     std::cout << "Reader thread " << std::this_thread::get_id() << " started\n";
        for (auto x : v) {
            sum += x;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
#ifdef SYNC
        std::cout << " Reader thread " << std::this_thread::get_id() << " ended\n";
        wrt.unlock_shared();
#endif // SYNC
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

#ifdef SYNC
        wrt.lock();
#endif // SYNC
        std::cout << " Writer thread " << std::this_thread::get_id() << " started\n";

        for (auto& x : v) {
            x = value;
            value = -value;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
#ifdef SYNC
        std::cout << " Writer thread " << std::this_thread::get_id() << " ended\n";
        wrt.unlock();
#endif // SYNC

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
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 4; i++) {
        std::cout << "Trial " << i << std::endl;
        std::vector<std::thread> mythreads;
        for (int i = 0; i < 2; i++) {
            Reader r1, r2;
            Writer w;
            std::thread t1(r1);
            std::thread t2(w);
            std::thread t3(r2);
            mythreads.push_back(std::move(t1));
            mythreads.push_back(std::move(t2));
            mythreads.push_back(std::move(t3));
        }
        //t1.join();
        //t2.join();
        for (auto& t : mythreads)
            t.join();
        std::cout << "----------------" << std::endl;
        
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<float, std::ratio<1, 1>>(end - start);
    std::cout << "total " << duration.count() << std::endl;
}
