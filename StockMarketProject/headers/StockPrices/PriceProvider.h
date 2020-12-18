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
            price_map_.insert(std::make_pair(std::move(id), price));
        }

        void remove_stock(std::string && id) {
            price_map_.erase(id);
        }

        std::shared_ptr<Price> get_price(std::string && id) const {
            const auto itr = price_map_.find(id);
            std::shared_ptr<Price> result;
            if (itr != price_map_.end())
            {
                result = std::make_shared<Price>(itr->second);
            }

            return result;
        }

        void update_stock(std::string &&id, Price&& newPrice) {
            auto ir = price_map_.find(id);
            ir->second = newPrice;
        }
    };
}

#endif //STOCKMARKETPROJECT_PRICEPROVIDER_H
