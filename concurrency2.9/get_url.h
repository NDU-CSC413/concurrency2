#pragma once
#pragma once
#include <memory>
#include <string>

struct MemoryStruct {
    char* memory;
    size_t size;
};
std::unique_ptr<MemoryStruct> get_url(std::string);
