//
// Created by zylyo on 08/03/2026.
//

#ifndef PROJECTSKYBLOCK_PRICEOBJECT_H
#define PROJECTSKYBLOCK_PRICEOBJECT_H

#include "dataToken.h"

namespace Price {
    class PriceObject {


        public:
            PriceObject();
            PriceObject(double avg_price, double high, double low, long long vol);
            PriceObject(const dataToken& data_token , bool isBuy);
            PriceObject(const PriceObject& other);
            void UpdateBuy(const dataToken& data_token);
            void UpdateSell(const dataToken &token);


            double getOpenPrice() const;
            double getHighPrice() const;
            double getLowPrice() const;
            double getClosePrice() const;
            long long getVolume() const;
            double getAveragePrice() const;


        private:

        double open_price;
        double high_price;
        double low_price;
        double close_price;
        long long active_volume;
        double price_sum;
        int ticks;



    };
} // Price

#endif //PROJECTSKYBLOCK_PRICEOBJECT_H