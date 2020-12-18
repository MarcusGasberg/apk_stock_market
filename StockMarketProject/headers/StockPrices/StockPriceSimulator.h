//
// Created by stud on 12/17/20.
//

#ifndef STOCKMARKETPROJECT_STOCKPRICESIMULATOR_H
#define STOCKMARKETPROJECT_STOCKPRICESIMULATOR_H
#include "iostream"
#include "PriceProvider.h"
#include "algorithm"
#include "../Models/Price.h"
#include "random"
#include "thread"
#include "chrono"
#include "../Helpers/UserLiterals.h"
#include "math.h"


namespace stock
{
    using namespace std::chrono_literals;
    class StockPriceSimulator {
        // Updates prices every 10th min.
        void update_prices(std::shared_ptr<PriceProvider> provider, std::atomic<bool> & termination_signal){
            while(!termination_signal){
            "Updating prices..."_print;
            std::map<std::string, Price> map = provider.get()->getPriceMap();
            std::for_each(map.begin(), map.end(), [&](std::pair<std::string, Price> && m){
                float pct = ((float)(rand() % 20 + 90) / 100);
                auto new_price = (float)m.second.price_ * ((float)(rand() % 20 + 90) / 100);
                provider.get()->update_stock(std::move(m.first), std::round(new_price));
            });
            std::this_thread::sleep_for(10s);
            }
        }
    public:
        StockPriceSimulator() = default;

        std::thread start_simulation(std::shared_ptr<PriceProvider> provider, std::atomic<bool> & termination_signal){
            auto bound = std::bind(&StockPriceSimulator::update_prices, this, provider, std::ref(termination_signal));
            return std::thread(bound);
        }
    };
}

#endif //STOCKMARKETPROJECT_STOCKPRICESIMULATOR_H
