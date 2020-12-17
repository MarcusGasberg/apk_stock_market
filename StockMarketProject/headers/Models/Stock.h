//
// Created by stud on 12/7/20.
//

#ifndef STOCKMARKETPROJECT_STOCK_H
#define STOCKMARKETPROJECT_STOCK_H
#include <iostream>

namespace stock
{
    class Stock
    {
    private:
        std::string stockId;
        uint64_t amount_{};
    public:

        Stock() = default;

        Stock(std::string&& stock_id,  const uint64_t amount)
            : stockId(std::move(stock_id)),
              amount_(amount)
        {
        }

        const std::string &getStockId() const {
            return stockId;
        }

        void setStockId(const std::string &stockId) {
            Stock::stockId = stockId;
        }

        uint64_t getAmount() const {
            return amount_;
        }

        void setAmount(uint64_t amount) {
            Stock::amount_ = amount;
        }

        operator std::string() const
        {
            std::stringstream ss;
            ss << "ID: " << getStockId() << ", Amount: " << getAmount() << "\n";
            return ss.str();
        }
    };
}
#endif //STOCKMARKETPROJECT_STOCK_H
