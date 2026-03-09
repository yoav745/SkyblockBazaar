//
// Created by zylyo on 04/03/2026.
//

#ifndef PROJECTSKYBLOCK_ITEMHOUR_H
#define PROJECTSKYBLOCK_ITEMHOUR_H

#include <string>
#include <nlohmann/json.hpp>
#include "dataToken.h"
#include "PriceObject.h"
namespace hourNamespace {
    #define HOUR_ENDED 1
    class itemHour {

    public:

        itemHour();
        explicit itemHour(const dataToken& data_token);
        int update(const dataToken& token);

        itemHour(const Price::PriceObject& otherBuy,const  Price::PriceObject& ootherSell , long long timestamp);
        [[nodiscard]] double getSellHighPrice() const;
        [[nodiscard]] double getSellLowPrice() const;
        [[nodiscard]] double getSellAveragePrice() const;
        [[nodiscard]] long long getTimestamp() const;
        [[nodiscard]] long long getSellVolume() const;

        [[nodiscard]] double getBuyHighPrice() const;
        [[nodiscard]] double getBuyLowPrice() const;
        [[nodiscard]] double getBuyAveragePrice() const;
        [[nodiscard]] long long getBuyVolume() const;

    private:
        long long start_timestamp;
        std::string product_id;
        Price::PriceObject sell;
        Price::PriceObject buy;
    };
}
#endif //PROJECTSKYBLOCK_ITEMHOUR_H