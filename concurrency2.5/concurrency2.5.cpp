/*****************************************************************//**
 * @file   concurrency2.5.cpp
 * @brief  Thread safe stack
 * TODO: more explanation is needed
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

#include <iostream>
#include <mutex> 
#include <stack>
#include <exception>
#include <thread>
#include <vector>
#include <set>
#include <chrono>

//#define SAFE
template<typename T> class threadsafe_stack {
private:
std::stack<T> data;     
std::mutex m; 
public:
    threadsafe_stack() {}
   
    void push(T new_value){
#ifdef SAFE
        std::lock_guard<std::mutex> lock(m);       
#endif 
        data.push(new_value);
    }
    
    void pop(T & value) {
#ifdef SAFE
        std::lock_guard<std::mutex> lock(m);  
#endif 
        if (data.empty()) throw std::exception("empty stack");
        value = data.top();        
        data.pop();
    }
    bool empty() const {
#ifdef SAFE
        std::lock_guard<std::mutex> lock(m);
#endif 
        return data.empty();
    }
};

std::mutex vm;
std::vector<int> values;
threadsafe_stack<int> s;
void pop_and_print(int label) {
    int val;
    for (int i = 0; i < 100; ++i) {
        s.pop(val);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        //std::cout << "thread " << label << "->" << val << "\n";
        vm.lock();
        values.push_back(val);
        vm.unlock();
    }
}

int main()
{
    const int num_threads = 5;
    std::vector<std::thread> mythreads;
    
    for (int i = 0; i < 1000; ++i)
        s.push(i);
    for (int i = 0; i < num_threads; ++i) {
        std::thread t = std::thread(pop_and_print, i);
        mythreads.push_back(std::move(t));
    }
    for (auto& t : mythreads)t.join();
    std::set<int> set(values.begin(),values.end());
    
    std::cout << set.size() << ",";
    std::cout << std::endl;
}

