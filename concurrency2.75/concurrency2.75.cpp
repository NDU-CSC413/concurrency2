#include <iostream>
#include <thread>
#include <barrier>
#include <chrono>
#include<random>


int num_threads = 10;
using Duration = std::chrono::seconds;
std::barrier barrier{ num_threads };

void threadf(int i, Duration d) {
    if (i % 2 == 0) {
        std::this_thread::sleep_for(d);
        std::cout << i << " started\n";
        barrier.arrive_and_wait();
        std::cout << i << " ended\n";
    }
  //  else
    //    barrier.arrive_and_drop();

}
int main()
{
    std::random_device rd;
    std::uniform_int_distribution<> dist(1, 5);
    std::vector<std::thread> mythreads;
    for (int i = 0; i < num_threads - 1; ++i)
        mythreads.push_back(
            std::thread(threadf, i, Duration(dist(rd)))
        );
    /* let them wait for the last thread */
    mythreads.push_back(
        std::thread(threadf, num_threads - 1, Duration(15))
    );
    for (auto& t : mythreads)
        t.join();
}