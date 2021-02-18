#include <iostream>

#include <curl/curl.h>
#include <memory>
#include "get_url.h"

/* TODO: convert to smart pointer*/
static size_t
WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
std::unique_ptr<MemoryStruct>
get_url(std::string url) {
    CURL* curl_handle;
    CURLcode res;

    MemoryStruct* buf = new MemoryStruct;
    buf->memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
    buf->size = 0;    /* no data at this point */
    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)buf);
    res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK) {
        std::cerr << "error\n";
    }
    /// else std::cerr << buf->size;
     /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();

    return  std::unique_ptr<MemoryStruct>(buf);
}
