#include <iostream>
#include <future>
#include <fstream>
#include <thread>
#include <random>
#include "get_url.h"
std::random_device rd;
std::uniform_int_distribution<> dist(1, 10);
using namespace std::literals::chrono_literals;
int main()
{
   // std::string  url = "https://hikmatfarhat-ndu.github.io/CSC325/files/flows.pdf";
    std::string url = "http://www.google.com";
    int total = 0, sum = 0;
    int x;
    auto   fut = std::async(get_url, url);
    std::future_status status;
    do {
        ++total;
        status = fut.wait_for(0.3s);
        std::cout << "enter a number\n";
        int g = dist(rd);
        std::cin >> x;
        if (g == x) {
            std::cout << "you guessed correct\n";
            ++sum;
        }
        else {
            std::cout << " you guessed " << x << " value is " << g << "\n";
        }


    } while (status != std::future_status::ready);
    std::cout << "----------------------------\n";
    std::cout << "----------------------------\n";

    std::cout << "You guessed " << sum << " out of " << total << "\n";
    std::cout << "Writing data to file\n";
    auto  data = fut.get();
    std::ofstream output;
    output.open("greedy.pdf", std::ios_base::binary);
    output.write(data->memory, data->size);
    output.close();
}

