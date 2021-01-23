#include <iostream>
#include <memory>


#define UNIQUE
struct Test {

#if defined  UNIQUE
    std::unique_ptr<int> res;
#else 
    int* resource = nullptr;
#endif 
    Test() {

#if defined UNIQUE
        res = std::make_unique<int>(10);
#else 
        resource = new int(10);
#endif 
        std::cout << "ctor: allocate\n";
    }
    ~Test() {
#if not defined UNIQUE
        if (resource) {
            delete resource;
            std::cout << "deallocate\n";
        }
#endif 
        std::cout << "dtor\n";
    }
};

#if defined UNIQUE
//Note the return as reference since
// a std::unique_ptr cannot be copied
std::unique_ptr<int>& mod(std::unique_ptr<int> & p) {
#else
int* mod(int* p) {
#endif 
    * p = 14;
    return p;
}
int* alloc() {
    return new int(33);
}

int main() {
    Test* p = new Test();
    int* r = alloc();

#if defined UNIQUE
    //transfer of ownership using std::move because
// std::unique_ptr cannot be copied
    std::unique_ptr<int> t = std::move(mod(p->res));
#else
    int* t = mod(p->resource);
    delete p;
#endif 
    delete p;
    delete r;
    std::cout << *t << std::endl;
#if not defined UNIQUE
    delete t;
#endif 
    std::cout << "done\n";
}