#include "dataToken.h"
#include <iostream>

// Constructor Implementation
dataToken::dataToken(const nlohmann::json& item_data) {
    product_id = item_data["product_id"];

    // Hypixel stores the current summary in "quick_status"
    auto status = item_data["quick_status"];

    buy_price = status["buyPrice"];
    sell_price = status["sellPrice"];
    buy_volume = status["buyVolume"];
    sell_volume = status["sellVolume"];
}

// Display Method Implementation
void dataToken::display() const {
    std::cout << "--- " << product_id << " ---\n"
              << "Instant Buy Price : $" << buy_price << "\n"
              << "Instant Sell Price: $" << sell_price << "\n"
              << "Bid-Ask Spread    : $" << (buy_price - sell_price) << "\n\n";
}