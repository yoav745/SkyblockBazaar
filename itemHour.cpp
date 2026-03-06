
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
    itemHour::itemHour(long long ts, double avg_price, double high, double low, long long vol) : itemHour() {
        start_timestamp = ts;
        high_price = high;
        low_price = low;
        active_volume = vol;

        // THE MATH TRICK: Set count to 1 and sum to the average.
        // Now getAveragePrice() will return exactly the average!
        tick_count = 1;
        price_sum = avg_price;
    }
    itemHour::itemHour(const dataToken& token) : itemHour() {
        update(token);
    }


    int itemHour::update(const dataToken& token) {

        if (tick_count == 0) {
            start_timestamp = token.getTimestamp();
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

        if (start_timestamp != 0 && token.getTimestamp() >= start_timestamp + 3600000) {
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

    long long itemHour::getTimestamp() const { return start_timestamp; }
    long long itemHour::getVolume() const { return active_volume; }
    double itemHour::getHighPrice() const { return high_price; }

    double itemHour::getLowPrice() const { return low_price; }

    double itemHour::getAveragePrice() const {
        if (tick_count == 0) return 0.0;
        return price_sum / tick_count;
    }

}