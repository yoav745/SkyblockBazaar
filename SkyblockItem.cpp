//
// Created by zylyo on 06/03/2026.
//

#include "SkyblockItem.h"

#include <iostream>

namespace SkyblockItems {

    SkyblockItem::SkyblockItem(const std::string &id , DatabaseManager* db)
    : product_id(id), average_price(0) , db_manager(db)
    {
        if (db_manager != nullptr) {
            std::vector<hourNamespace::itemHour> saved_history = db_manager->loadHistory(product_id);

            for (const auto& past_hour : saved_history) {
                // Re-run your exact O(1) math pipeline!
                addHourToHistory(past_hour);
            }
            if (!history.empty() && product_id == "ENCHANTED_DIAMOND_BLOCK") {
                std::cout << "\n[DATABASE RESTORE] " << product_id
                          << " | Recovered " << history.size() << " hours."
                          << " | Rebuilt Average: " << average_price << "\n\n";
            }
        }
    }



    void SkyblockItem::processTick(const dataToken &live_tick) {

        checkBuyOpportunity(live_tick.getBuyPrice());
        addItemToHour(live_tick);


    }
    void SkyblockItem::checkBuyOpportunity(double current_price) const {

        if (current_price > 0 && current_price < average_price * 0.8 && average_price - current_price > 1000) {
            std::cout << "BUY " << product_id << std::endl <<" The Price is: " << current_price << std::endl << " While the average price is " << average_price << std::endl;
        }
    }


    void SkyblockItem::addHourToHistory(const hourNamespace::itemHour& item_hour) {
        if(history.size() >= MAX_HISTORY) {
            long double historySum = average_price * history.size();
            historySum -= history.back().getAveragePrice();
            historySum += item_hour.getAveragePrice();
            average_price = historySum / static_cast<double>(history.size());

            if(db_manager != nullptr) {
                // Tell SQL to delete the item we are about to pop
                db_manager->deleteHour(product_id, history.back().getTimestamp());
            }
            history.pop_back();
            history.push_front(item_hour);
        }
        else {
            long double historySum = average_price * history.size();
            history.push_front(item_hour);
            average_price = ( historySum + item_hour.getAveragePrice() )  / history.size();
        }
    }

    void SkyblockItem::addItemToHour(const dataToken &data) {
        if(active_hour.update(data) == 1) {
            addHourToHistory(active_hour);
            if (db_manager != nullptr) {
                db_manager->insertHourData(product_id, active_hour);
            }
            active_hour = hourNamespace::itemHour();
            active_hour.update(data);
        }
    }

}
