//
// Created by stud on 12/17/20.
//

#ifndef STOCKMARKETPROJECT_PRICEPROVIDER_H
#define STOCKMARKETPROJECT_PRICEPROVIDER_H

#include <shared_mutex>
#include "map"
#include "string"
#include "algorithm"

namespace stock {
    class PriceProvider {
        mutable std::shared_mutex price_lock_;
        typedef std::map<std::string, Price> pm_t;
        pm_t price_map_;
    public:
        PriceProvider() {}

        void add_stock(std::string &&id, Price&& price) {
            std::unique_lock lock(price_lock_);
            price_map_.insert(std::make_pair(std::move(id), std::move(price)));
        }

        void remove_stock(std::string && id) {
            std::unique_lock lock(price_lock_);
            price_map_.erase(id);
        }

        Price get_price(std::string && id) const {
            std::shared_lock lock(price_lock_);
            return price_map_.find(id)->second;
        }

        void update_stock(std::string &&id, Price&& newPrice) {
            std::unique_lock lock(price_lock_);
            auto ir = price_map_.find(id);
            ir->second = newPrice;
        }
    };
}

#endif //STOCKMARKETPROJECT_PRICEPROVIDER_H
