//
// Created by zylyo on 04/03/2026.
//

#ifndef PROJECTSKYBLOCK_ITEMHOUR_H
#define PROJECTSKYBLOCK_ITEMHOUR_H

#include <string>
#include <nlohmann/json.hpp>
#include "dataToken.h"

namespace hourNamespace {
    #define HOUR_ENDED 1
    class itemHour {

    public:

        itemHour();
        explicit itemHour(const dataToken& data_token);
        int update(const dataToken& token);


        [[nodiscard]] double getHighPrice() const;
        [[nodiscard]] double getLowPrice() const;
        [[nodiscard]] double getAveragePrice() const;


    private:
        long long start_timestamp;
        std::string product_id;

        double open_price;
        double high_price;
        double low_price;
        double close_price;


        double price_sum;
        int tick_count;
        long long active_volume;

    };
}
#endif //PROJECTSKYBLOCK_ITEMHOUR_H