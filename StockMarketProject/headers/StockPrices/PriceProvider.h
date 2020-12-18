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
        std::map<std::string, Price> price_map_;
        mutable std::shared_mutex price_lock_;
    public:
        PriceProvider() = default;

        void add_stock(std::string &&id, Price&& price) {
            std::unique_lock lock(price_lock_);
            price_map_.insert(std::make_pair(std::move(id), std::move(price)));
        }

        void remove_stock(std::string && id) {
            std::unique_lock lock(price_lock_);
            price_map_.erase(id);
        }

        std::shared_ptr<Price> get_price(std::string && id) const {
            std::shared_lock lock(price_lock_);
            auto itr = price_map_.find(id);
            std::shared_ptr<Price> result;
            if (itr != price_map_.end())
            {
                result = std::make_shared<Price>(itr->second);
            }
            return result;
        }

        // TODO: Make this friend only, others should
        std::map<std::string, Price> &getPriceMap() {
            return price_map_;
        }

        void update_stock(std::string &&id, Price&& newPrice) {
            std::unique_lock lock(price_lock_);
            auto ir = price_map_.find(id);
            ir->second = newPrice;
        }
    };
}

#endif //STOCKMARKETPROJECT_PRICEPROVIDER_H
