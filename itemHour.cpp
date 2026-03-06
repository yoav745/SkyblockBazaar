
#include "itemHour.h"
#include "dataToken.h"

#define HOUR_ENDED_ERROR 1
#define SUCCESS_CODE 0

namespace hourNamespace {
    itemHour::itemHour() {
        start_timestamp = 0;
        product_id = "UNKNOWN";
        open_price = 0.0;
        high_price = 0.0;
        low_price = 0.0;
        close_price = 0.0;
        price_sum = 0.0;
        tick_count = 0;
        active_volume = 0;
    }

    itemHour::itemHour(const dataToken& token) {


        start_timestamp = 0;
        product_id = token.getProductId();
        open_price = token.getBuyPrice();
        high_price = token.getBuyPrice();
        low_price = token.getBuyPrice();
        close_price = token.getBuyPrice();
        price_sum = token.getBuyPrice();
        tick_count = 1;
        active_volume = token.getBuyVolume();
    }
    itemHour::itemHour(const nlohmann::json& item_data)
        : itemHour(dataToken(item_data)) {}

    int itemHour::update(const dataToken& token, long long current_time) {

        if (tick_count == 0) {
            start_timestamp = current_time;
            product_id = token.getProductId();
            open_price = token.getBuyPrice();
            high_price = token.getBuyPrice();
            low_price = token.getBuyPrice();
            tick_count = 1;
            close_price = token.getBuyPrice();
            price_sum = token.getBuyPrice();
            active_volume = token.getBuyVolume();
            return SUCCESS_CODE;
        }

        if (start_timestamp != 0 && current_time >= start_timestamp + 1000) {
            return HOUR_ENDED_ERROR;
        }
        double current_price = token.getBuyPrice();
        if (current_price > high_price) {
            high_price = current_price;
        }
        if (current_price < low_price) {
            low_price = current_price;
        }
        close_price = current_price;
        price_sum += current_price;
        tick_count++;
        active_volume = token.getBuyVolume();
        return SUCCESS_CODE;
    }

    double itemHour::getHighPrice() const { return high_price; }

    double itemHour::getLowPrice() const { return low_price; }

    double itemHour::getAveragePrice() const {
        if (tick_count == 0) return 0.0;
        return price_sum / tick_count;
    }

}