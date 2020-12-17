//
// Created by stud on 12/17/20.
//

#ifndef STOCKMARKETPROJECT_STOCKMARKETSIMULATOR_H
#define STOCKMARKETPROJECT_STOCKMARKETSIMULATOR_H

#include <mutex>
#include "PriceProvider.h"

namespace stock {
    class StockMarketSimulator {
        PriceProvider & price_provider;

        void update_stock_price(){

        }
    };
}

#endif //STOCKMARKETPROJECT_STOCKMARKETSIMULATOR_H
