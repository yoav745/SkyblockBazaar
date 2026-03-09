//
// Created by zylyo on 08/03/2026.
//

#include "PriceObject.h"

#include "itemHour.h"
#define SUCCESS_CODE 1
namespace Price {


    PriceObject::PriceObject()
        : open_price(0.0) , high_price(0.0) , low_price(0.0), close_price(0.0) , active_volume(0) , price_sum(0) , ticks(0)
    {

    }

    PriceObject::PriceObject(const PriceObject& other)
        : open_price(other.getOpenPrice()) , high_price(other.getHighPrice()) ,
        low_price(other.getLowPrice()) , close_price(other.getClosePrice()) ,
        active_volume(other.getVolume()) , price_sum(other.getAveragePrice())
        , ticks(1)
    {

    }
    PriceObject::PriceObject(double priceSum, double high, double low, long long vol)
        : price_sum(priceSum) , high_price(high) , low_price(low) , active_volume(vol) , ticks(1)
    {

    }
    PriceObject::PriceObject(const dataToken &data_token , bool isBuy )
        : PriceObject()
    {
        if (isBuy) {
            UpdateBuy(data_token);
        } else {
            UpdateSell(data_token);
        }

    }

    void PriceObject::UpdateBuy(const dataToken &token) {
        if (ticks == 0) {
            open_price = token.getBuyPrice();
            high_price = token.getBuyPrice();
            low_price = token.getBuyPrice();
            ticks = 1;
            close_price = token.getBuyPrice();
            price_sum = token.getBuyPrice();
            active_volume = token.getBuyVolume();
            return;
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
        ticks++;
        active_volume = token.getBuyVolume();
    }
    void PriceObject::UpdateSell(const dataToken &token) {
        if (ticks == 0) {
            open_price = token.getSellPrice();
            high_price = token.getSellPrice();
            low_price = token.getSellPrice();
            ticks = 1;
            close_price = token.getSellPrice();
            price_sum = token.getSellPrice();
            active_volume = token.getSellVolume();
            return;
        }


        double current_price = token.getSellPrice();
        if (current_price > high_price) {
            high_price = current_price;
        }
        if (current_price < low_price) {
            low_price = current_price;
        }
        close_price = current_price;
        price_sum += current_price;
        ticks++;
        active_volume = token.getSellVolume();
    }
    double PriceObject::getOpenPrice() const {return open_price;}
    double PriceObject::getHighPrice() const {return high_price;}
    double PriceObject::getClosePrice() const {return close_price;}
    double PriceObject::getLowPrice() const {return low_price;}
    long long PriceObject::getVolume() const { return active_volume;}
    double PriceObject::getAveragePrice() const{
        if(ticks != 0) {
            return price_sum / (double)ticks;
        }
        return 0;
    }







} // Price