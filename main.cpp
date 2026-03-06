#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <curl/curl.h>
#include "lib/nlohmann/json.hpp"
#include "dataToken.h"
#include "DatabaseManager.h"
#include "SkyblockItem.h"

using json = nlohmann::json;

// cURL write callback
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    DatabaseManager db("market_data.db");
    // The "In-Memory Database": Maps the Product ID string to its Manager object
    std::unordered_map<std::string, SkyblockItems::SkyblockItem> market;

    std::cout << "=================================================\n";
    std::cout << "   INITIALIZING SKYBLOCK QUANTITATIVE ENGINE     \n";
    std::cout << "=================================================\n\n";

    // Track the last API update to prevent processing the same data twice
    long long last_processed_time = 0;
    int api_calls = 0;

    // Run indefinitely
    while (true) {
        std::string readBuffer;
        CURL* curl = curl_easy_init();

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.hypixel.net/v2/skyblock/bazaar");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            if (curl_easy_perform(curl) == CURLE_OK) {
                json data = json::parse(readBuffer);

                if (data["success"] == true) {
                    long long api_time = data["lastUpdated"];

                    // The Bazaar only updates every ~60 seconds.
                    // If the time hasn't changed, ignore it and wait.
                    if (api_time > last_processed_time) {
                        last_processed_time = api_time;
                        api_calls++;

                        int items_processed = 0;

                        // Iterate through every single item in the Bazaar
                        for (auto& [item_name, item_data] : data["products"].items()) {

                            // 1. If we've never seen this item before, create a new manager for it
                            if (market.find(item_name) == market.end()) {
                                market.emplace(item_name, SkyblockItems::SkyblockItem(item_name , &db));
                            }

                            // 2. Create the data token
                            dataToken tick(item_data, api_time);

                            // 3. Route the tick to the correct manager
                            market.at(item_name).processTick(tick);

                            items_processed++;
                        }

                        // --- THE CONSOLE DASHBOARD ---
                        // We use \r to overwrite the current line instead of scrolling forever
                        std::cout << "\r[ ENGINE STATUS ] Active Trackers: " << market.size()
                                  << " | API Pings: " << api_calls
                                  << " | Last Update: " << api_time << std::flush;
                    }
                }
            }
            curl_easy_cleanup(curl);
        }

        // Poll the API every 15 seconds.
        // We poll faster than the 60s update to catch the price change the moment it happens.
        std::this_thread::sleep_for(std::chrono::seconds(15));
    }

    return 0;
}