//
// Created by zylyo on 06/03/2026.
//

#include "SkyblockItem.h"

#include <iostream>

namespace SkyblockItems {

    SkyblockItem::SkyblockItem(const std::string &id)
    : product_id(id), average_price(0)
    {
        // No need to initialize other elements
    }

    void SkyblockItem::processTick(const dataToken &live_tick, long long current_time) {

        checkBuyOpportunity(live_tick.getBuyPrice());
        addItemToHour(live_tick,current_time);


    }
    void SkyblockItem::checkBuyOpportunity(double current_price) const {

        if (current_price < average_price * 0.8 && average_price - current_price > 1000) {
            std::cout << "BUY " << product_id << std::endl <<" The Price is: " << current_price << std::endl << " While the average price is " << average_price << std::endl;
        }
    }


    void SkyblockItem::addHourToHistory(const hourNamespace::itemHour& item_hour) {
        if(history.size() >= MAX_HISTORY) {
            long double historySum = average_price * history.size();
            historySum -= history.back().getAveragePrice();
            historySum += item_hour.getAveragePrice();
            average_price = historySum / static_cast<double>(history.size());
            history.pop_back();
            history.push_front(item_hour);
        }
        else {
            long double historySum = average_price * history.size();
            history.push_front(item_hour);
            average_price = ( historySum + item_hour.getAveragePrice() )  / history.size();
        }
    }

    void SkyblockItem::addItemToHour(const dataToken &data , long long current_time) {
        if(active_hour.update(data,current_time) == 1) {
            addHourToHistory(active_hour);
            active_hour = hourNamespace::itemHour();
            active_hour.update(data,current_time);
        }
    }

}
