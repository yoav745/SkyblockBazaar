
#include "itemHour.h"
#include "dataToken.h"

#define HOUR_ENDED_ERROR 1
#define SUCCESS_CODE 0

namespace hourNamespace {
    itemHour::itemHour()
    : start_timestamp(0) , product_id("UNKOWN") , sell() , buy()
    {

    }
    itemHour::itemHour(const Price::PriceObject& otherBuy ,const Price::PriceObject& otherSell , long long ts)
        : start_timestamp(ts) , sell(otherSell) , buy(otherBuy)
    {

    }
    itemHour::itemHour(const dataToken& token) : itemHour() {
        update(token);
    }


    int itemHour::update(const dataToken& token) {

        if(start_timestamp == 0) {
            start_timestamp = token.getTimestamp();
        }
        if (start_timestamp != 0 && token.getTimestamp() >= start_timestamp + 3600000) {
            return HOUR_ENDED_ERROR;
        }
        sell.UpdateSell(token);
        buy.UpdateBuy(token);
        return SUCCESS_CODE;
    }

    long long itemHour::getTimestamp() const { return start_timestamp; }

    long long itemHour::getSellVolume() const { return sell.getVolume(); }
    double itemHour::getSellHighPrice() const { return sell.getHighPrice(); }
    double itemHour::getSellAveragePrice() const { return sell.getAveragePrice();}
    double itemHour::getSellLowPrice() const { return sell.getLowPrice();}

    double itemHour::getBuyLowPrice() const { return buy.getLowPrice();}
    long long itemHour::getBuyVolume() const { return buy.getVolume(); }
    double itemHour::getBuyHighPrice() const { return buy.getHighPrice(); }
    double itemHour::getBuyAveragePrice() const { return buy.getAveragePrice();}

}