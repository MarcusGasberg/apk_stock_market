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
        std::string stockId;
        int amount_{};
        std::shared_ptr<Price> price_;
    public:

        Stock() = default;

        Stock(std::string&& stock_id,  const int amount)
            : stockId(std::move(stock_id)),
              amount_(amount)
        {
        }

        std::string getStockId() const {
            return stockId;
        }

        void setStockId(const std::string &stockId) {
            Stock::stockId = stockId;
        }

        int getAmount() const {
            return amount_;
        }

        void setAmount(int amount) {
            Stock::amount_ = amount;
        }

        const Price getPrice() const {
            return *price_;
        }

        void setPrice(std::shared_ptr<Price> price) {
            price_ = price;
        }

        friend std::ostream &operator<<(std::ostream &os, const Stock &stock1) {
            os << "ID: " << stock1.getStockId() << ", Amount: " << stock1.getAmount() << ", Price: " << stock1.getPrice().price_ << "\n";
            return os;
        }
    };
}
#endif //STOCKMARKETPROJECT_STOCK_H
