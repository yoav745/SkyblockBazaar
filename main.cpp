#include <iostream>
#include <thread>
#include <chrono>
#include <curl/curl.h>
#include "lib/nlohmann/json.hpp"
#include "dataToken.h"
#include "SkyblockItem.h"

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    std::string target_item = "ENCHANTED_DIAMOND_BLOCK";

    // 1. Instantiate the Algorithmic Manager
    SkyblockItems::SkyblockItem item_manager(target_item);

    long long simulated_time = 0;
    int loop_count = 0;
    auto start_test = std::chrono::steady_clock::now();

    std::cout << "Starting Algorithmic Engine Test for: " << target_item << "\n";
    std::cout << "Building historical baseline... Please wait.\n" << std::endl;

    // Run the test for 45 seconds real-time
    while (std::chrono::steady_clock::now() - start_test < std::chrono::seconds(45)) {
        std::string readBuffer;
        CURL* curl = curl_easy_init();

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.hypixel.net/v2/skyblock/bazaar");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            if (curl_easy_perform(curl) == CURLE_OK) {
                json data = json::parse(readBuffer);

                if (data["success"] == true) {
                    if (simulated_time == 0) simulated_time = data["lastUpdated"];
                    simulated_time += 2000; // Force 2 seconds to pass every tick
                    loop_count++;

                    // Extract the raw item data
                    json item_json = data["products"][target_item];

                    // =========================================================
                    // THE CRASH INJECTOR (For testing the Z-Score Math)
                    // =========================================================
                    // After 21 loops, we have built a solid baseline in the std::deque.
                    // On the 22nd loop, we simulate a massive market crash!
                    bool gate = false;
                    if (loop_count == 22) {
                        std::cout << "\n[TEST INJECTION] Simulating a massive 30% market crash..." << std::endl;
                        double real_price = item_json["quick_status"]["buyPrice"];
                        item_json["quick_status"]["buyPrice"] = real_price * 0.70; // Drop price by 30%
                        gate = true;
                    }

                    if(gate) {
                        std::cout << "";
                    }
                    // 2. Create the token and feed it to the algorithm
                    dataToken tick(item_json , data["lastUpdated"]);
                    std::cout << "Price " << tick.getBuyPrice() << std::endl;
                    // The manager handles the math, the archiving, and the alerts internally
                    item_manager.processTick(tick);

                }
            }
            curl_easy_cleanup(curl);
        }

        // Sleep for 1 second real-time to respect API rate limits
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "\n--- ENGINE TEST COMPLETE ---" << std::endl;
    return 0;
}