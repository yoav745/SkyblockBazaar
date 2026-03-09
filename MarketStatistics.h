//
// Created by zylyo on 09/03/2026.
//

#ifndef PROJECTSKYBLOCK_MARKETSTATISTICS_H
#define PROJECTSKYBLOCK_MARKETSTATISTICS_H


#include <deque>
#include "itemHour.h"
namespace Statistics {
    class MarketStatistics {
        public:
        static double calculateVolatility(const std::deque<hourNamespace::itemHour>& history, double currentMean);
        static double calculatePriceMomentum(const std::deque<hourNamespace::itemHour>& history);

        // You can even pass just the basic averages if you don't need the whole history
        static bool isOutlier(double current_price, double average_price);
    };
} // Statistics

#endif //PROJECTSKYBLOCK_MARKETSTATISTICS_H