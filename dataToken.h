#ifndef DATATOKEN_H
#define DATATOKEN_H

#include <string>
#include <nlohmann/json.hpp>

class dataToken {
public:


    // Constructor declaration
    explicit dataToken(const nlohmann::json& item_data);

    // Display function declaration
    void display() const;

    std::string getProductId() const;
    double getBuyPrice() const;
    double getSellPrice() const;
    long getBuyVolume() const;
    long getSellVolume() const;

    private:

    std::string product_id;
    double buy_price;  // The current price to INSTANT BUY
    double sell_price; // The current price to INSTANT SELL
    long buy_volume;   // Total active buy orders
    long sell_volume;  // Total active sell offers
};

#endif // DATATOKEN_H