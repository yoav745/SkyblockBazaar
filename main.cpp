#include <iostream>
#include <curl/curl.h>
#include "lib/nlohmann/json.hpp"
#include "dataToken.h"


using json = nlohmann::json;
// This is a required helper function for libcurl to save the downloaded data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    std::cout << "Fetching live data from Hypixel Bazaar...\n";

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.hypixel.net/v2/skyblock/bazaar");

        // Tell curl to use our helper function to save the data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Execute the request
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "Failed to fetch data: " << curl_easy_strerror(res) << "\n";
            return 1;
        }
    }

    // Now we parse the raw string into a JSON object
    json bazaar_data = json::parse(readBuffer);

    // Check if the API request was successful
    if (bazaar_data["success"] == true) {

        // Let's grab the data for Enchanted Diamond Blocks
        json edb_json = bazaar_data["products"]["ENCHANTED_DIAMOND_BLOCK"];

        // Create your dataToken directly from the JSON
        dataToken myItem(edb_json);
        // Print it out!
        myItem.display();

    } else {
        std::cout << "API request failed or rate limited.\n";
    }

    return 0;
}