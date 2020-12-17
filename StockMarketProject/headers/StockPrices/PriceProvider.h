//
// Created by stud on 12/17/20.
//

#ifndef STOCKMARKETPROJECT_PRICEPROVIDER_H
#define STOCKMARKETPROJECT_PRICEPROVIDER_H
#include "map"
#include "string"
#include "algorithm"

namespace stock {
    class PriceProvider {
        typedef std::map<std::string, Price> pm_t;
        pm_t price_map_;
    public:
        PriceProvider() {}

        void add_stock(std::string &&id, Price&& price) {
            price_map_.insert(std::make_pair(std::move(id), std::move(price)));
        }

        void remove_stock(std::string && id) {
            price_map_.erase(id);
        }

        Price get_price(std::string && id) const {
            return price_map_.find(id)->second;
        }

        void update_stock(std::string &&id, Price&& newPrice) {
            auto ir = price_map_.find(id);
            ir->second = newPrice;
        }
    };
}

#endif //STOCKMARKETPROJECT_PRICEPROVIDER_H
