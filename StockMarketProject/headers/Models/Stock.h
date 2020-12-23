//
// Created by stud on 12/7/20.
//

#ifndef STOCKMARKETPROJECT_STOCK_H
#define STOCKMARKETPROJECT_STOCK_H
#include <iostream>
#include "Price.h"

namespace stock
{
    class Stock
    {
    private:
        std::string stock_id;
        std::string stock_provider_id_;
        int amount_{};
        std::shared_ptr<Price> price_;
    public:

        Stock() = default;

        Stock(std::string&& stock_id,  const int amount, std::string && stock_provider_id)
            : stock_id(std::move(stock_id)),
              amount_(amount),
              stock_provider_id_(std::move(stock_provider_id))
        {
        }

        std::string getStockId() const {
            return stock_id;
        }

        void setStockId(const std::string &stockId) {
            Stock::stock_id = stockId;
        }

        int getAmount() const {
            return amount_;
        }

        void setAmount(int amount) {
            Stock::amount_ = amount;
        }

        const std::string &getStockProviderId() const {
            return stock_provider_id_;
        }

        void setStockProviderId(const std::string &stockProviderId) {
            stock_provider_id_ = stockProviderId;
        }

        std::shared_ptr<Price> getPrice() const {
            return price_;
        }

        void setPrice(std::shared_ptr<Price> price) {
            price_ = price;
        }

        friend std::ostream &operator<<(std::ostream &os, const Stock &stock1) {
            os << "ID: " << stock1.getStockId() << ", Amount: " << stock1.getAmount() << ", Price: " << (stock1.getPrice() ? stock1.getPrice()->price_ : 0) << "\n";
            return os;
        }
    };
}
#endif //STOCKMARKETPROJECT_STOCK_H
