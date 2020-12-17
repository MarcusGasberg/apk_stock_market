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

namespace stock
{
    class StockPriceSimulator {
    public:
        StockPriceSimulator() = default;

        void startSimulation(std::shared_ptr<PriceProvider> provider){
            /*std::thread th([&provider](){
                std::map<std::string, Price> map = provider.get()->getPriceMap();
                std::for_each(map.begin(), map.end(), [](std::pair<std::string, Price> & m){
                    m.second.price_ += rand() % 120 + 80;
                });
            });
            return th;*/
        }
    };
}

#endif //STOCKMARKETPROJECT_STOCKPRICESIMULATOR_H
