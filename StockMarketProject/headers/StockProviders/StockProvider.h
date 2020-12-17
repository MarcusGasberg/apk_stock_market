//
// Created by stud on 12/15/20.
//

#ifndef STOCKMARKETPROJECT_STOCKPROVIDER_H
#define STOCKMARKETPROJECT_STOCKPROVIDER_H

#include <string>
#include "../Models/Stock.h"
#include "../StockPrices/PriceProvider.h"

namespace stock {
    class StockProvider {
    private:
        std::string name;
        std::string id;
        const PriceProvider & price_provider_;
    public:
        StockProvider(const std::string && name, std::string && id, const PriceProvider & price_provider) : name(std::move(name)), id(std::move(id)), price_provider_(price_provider) {}

        void stockHasBeenBought(Stock &&stock) {
            std::cout << "Stock " << stock.getStockId() << " was bought";
        }

        const std::string &getName() const {
            return name;
        }

        void setName(const std::string &name) {
            StockProvider::name = name;
        }

        const std::string &getId() const {
            return id;
        }

        void setId(const std::string &id) {
            StockProvider::id = id;
        }
    };
}
#endif //STOCKMARKETPROJECT_STOCKPROVIDER_H
