//
// Created by zylyo on 04/03/2026.
//

#ifndef PROJECTSKYBLOCK_ITEMHOUR_H
#define PROJECTSKYBLOCK_ITEMHOUR_H
#include "dataToken.h"

namespace hourNamespace {
    class itemHour {

    public:

        itemHour();
        explicit itemHour(const dataToken& data_token);
        explicit itemHour(const nlohmann::json& item_data);
        int update(const dataToken& token,long long current_time);


        double getHighPrice() const;
        double getLowPrice() const;
        double getAveragePrice() const;


    private:
        long long start_timestamp;
        std::string product_id;

        double open_price;
        double high_price;
        double low_price;
        double close_price;


        double price_sum;
        int tick_count;
        long active_volume;

    };
}
#endif //PROJECTSKYBLOCK_ITEMHOUR_H