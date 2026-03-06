#ifndef DATATOKEN_H
#define DATATOKEN_H

#include <string>
#include <nlohmann/json.hpp>

class dataToken {
public:


    // Constructor declaration
    explicit dataToken(const nlohmann::json& item_data , long long apiTimeStamp);

    // Display function declaration
    void display() const;

    std::string getProductId() const;

    double getBuyPrice() const;
    double getSellPrice() const;
    long long getBuyVolume() const;
    long long getSellVolume() const;
    long long getTimestamp() const;

    private:
    long long timestamp;
    std::string product_id;
    double buy_price;  // The current price to INSTANT BUY
    double sell_price; // The current price to INSTANT SELL
    long long buy_volume;   // Total active buy orders
    long long sell_volume;  // Total active sell offers
};

#endif // DATATOKEN_H