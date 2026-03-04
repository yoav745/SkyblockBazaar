#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <curl/curl.h>
#include "lib/nlohmann/json.hpp"
#include "dataToken.h"
#include "itemHour.h"

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    
}