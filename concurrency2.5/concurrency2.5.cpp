// concurrency2.5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <mutex> 
#include <stack>
#include <exception>

//struct empty_stack : std::exception {
//    const char* what() const throw();
//};

template<typename T> class threadsafe_stack {
private:
std::stack<T> data;     
//mutable 
std::mutex m; 
public:
    threadsafe_stack() {}
    threadsafe_stack(const threadsafe_stack& other) {
        std::lock_guard<std::mutex> lock(other.m);         
        data = other.data;
    }
  
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;     
    void push(T new_value){
        std::lock_guard<std::mutex> lock(m);         
        data.push(std::move(new_value));
    }
    std::shared_ptr<T> pop(){
        std::lock_guard<std::mutex> lock(m);         
        if (data.empty()) throw std::exception("empty stack");
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));     
        data.pop();
        return res;
    }
    void pop(T & value) {
        std::lock_guard<std::mutex> lock(m);        
        if (data.empty()) throw std::exception("empty stack");
        value = data.top();        
        data.pop();
    }
    bool empty() const {
        std::lock_guard<std::mutex> lock(m);         
        return data.empty();
    }
};
int main()
{
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
