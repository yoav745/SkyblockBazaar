//
// Created by zylyo on 06/03/2026.
//

#include "SkyblockItem.h"

#include <iostream>

namespace SkyblockItems {

    SkyblockItem::SkyblockItem(const std::string &id , DatabaseManager* db , double multipliernew)
    : product_id(id), averageBuyPrice(0), averageSellPrice(0) ,
    db_manager(db) , multiplier(multipliernew)
    {
        if (db_manager != nullptr) {
            std::vector<hourNamespace::itemHour> saved_history = db_manager->loadHistory(product_id);
            for (const auto& past_hour : saved_history) {
                addHourToHistory(past_hour);
            }
        }
    }



    void SkyblockItem::processTick(const dataToken &live_tick) {

        checkBuyOpportunity(live_tick);
        addItemToHour(live_tick);


    }
    void SkyblockItem::checkBuyOpportunity(const dataToken& currentToken) const {
        long double currentSellPrice = currentToken .getSellPrice();

        if(currentSellPrice < averageSellPrice * multiplier && currentSellPrice < averageSellPrice - 1000) {
            std::cout << "Buy: " << product_id << std::endl
            << "The usual price to buy is: " << averageSellPrice <<std::endl
            << "Yet now the price is: " << currentSellPrice <<std::endl
            << "You can insta sell usually for: " << averageBuyPrice <<std::endl
            << "Sell volume is: " << currentToken.getSellVolume() << std::endl
            << "Buy volume is: " << currentToken.getBuyVolume() << std::endl;
        }
    }

    void SkyblockItem::addHourToHistory(const hourNamespace::itemHour& item_hour) {
        if(history.size() >= MAX_HISTORY) {
            long double historyBuySum = averageBuyPrice * history.size();
            long double historySellSum = averageBuyPrice * history.size();



            historySellSum -= history.back().getSellAveragePrice();
            historyBuySum -= history.back().getBuyAveragePrice();

            historyBuySum += item_hour.getBuyAveragePrice();
            historySellSum += item_hour.getSellAveragePrice();

            averageSellPrice = historySellSum / static_cast<double>(history.size());
            averageBuyPrice = historyBuySum / static_cast<double>(history.size());


            if(db_manager != nullptr) {
                db_manager->deleteHour(product_id, history.back().getTimestamp());

            }
            history.pop_back();
            history.push_front(item_hour);
        }
        else {
            long double historyBuySum = averageBuyPrice * history.size();
            long double historySellSum = averageBuyPrice * history.size();

            history.push_front(item_hour);
            averageBuyPrice = ( historyBuySum + item_hour.getBuyAveragePrice() )  / history.size();
            averageSellPrice =  ( historySellSum + item_hour.getSellAveragePrice() )  / history.size();


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
