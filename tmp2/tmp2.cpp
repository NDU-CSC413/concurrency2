#include <iostream>
#include <memory>


#define SHARED
struct Test {

#if defined  SHARED
    std::shared_ptr<int> res;
#else 
    int* resource = nullptr;
#endif 
    Test() {

#if defined SHARED
        res = std::make_shared<int>(10);
#else 
        resource = new int(10);
#endif 
        std::cout << "ctor: allocate\n";
    }
    ~Test() {
#if not defined SHARED
        if (resource) {
            delete resource;
            std::cout << "deallocate\n";
        }
#endif 
        std::cout << "dtor\n";
    }
};

#if defined SHARED
std::shared_ptr<int> mod(std::shared_ptr<int> p) {
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

#if defined SHARED
    std::cout << "count before t=" << p->res.use_count() << "\n";
    std::shared_ptr<int> t = mod(p->res);
    std::cout << "count after t=" << p->res.use_count() << "\n";
#else
    int* t = mod(p->resource);
    delete p;
#endif 
    delete p;
    delete r;
#ifdef SHARED
    std::cout << "count after delete p=" << t.use_count() << "\n";
#endif 
    std::cout << *t << std::endl;
#if not defined SHARED
    delete t;
#endif 
    std::cout << "done\n";
}