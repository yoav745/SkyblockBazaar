//
// Created by zylyo on 09/03/2026.
//

#include "MarketStatistics.h"

namespace Statistics {



    double MarketStatistics::calculateVolatility(const std::deque<hourNamespace::itemHour> &history, double currentMean) {
        if (history.size() < 2) {
            return 0.0;
        }

        double sum_of_squared_differences = 0.0;

        // Loop through the deque to find how far each hour deviated from the mean
        for (const auto& hour : history) {
            double difference = hour.getSellAveragePrice() - currentMean;

            // Squaring it makes negative drops and positive spikes both count as "volatility"
            sum_of_squared_differences += (difference * difference);
        }

        // Calculate the Variance
        double variance = sum_of_squared_differences / history.size();

        // Return the Standard Deviation
        return std::sqrt(variance);

    }

} // Statistics