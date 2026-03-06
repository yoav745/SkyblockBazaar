//
// Created by zylyo on 06/03/2026.
//

#ifndef PROJECTSKYBLOCK_SKYBLOCKITEM_H
#define PROJECTSKYBLOCK_SKYBLOCKITEM_H

#include "dataToken.h"
#include "itemHour.h"
#include <string>
#include <deque>

namespace SkyblockItems {
    class SkyblockItem {


    public:
        explicit SkyblockItem(const std::string& id);
        void processTick(const dataToken& live_tick);

        [[nodiscard]] std::string getProductId() const;
        [[nodiscard]] size_t getHistorySize() const;

    private:
        std::string product_id;
        double average_price;
        std::deque<hourNamespace::itemHour> history;

        hourNamespace::itemHour active_hour;

        static constexpr size_t MAX_HISTORY = 168;
        void checkBuyOpportunity(double current_price) const;
        void addItemToHour(const dataToken& data);
        void addHourToHistory(const hourNamespace::itemHour& item_hour);

    };
} // SkyblockItems

#endif //PROJECTSKYBLOCK_SKYBLOCKITEM_H