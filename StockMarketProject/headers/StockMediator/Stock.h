//
// Created by stud on 12/7/20.
//

#ifndef STOCKMARKETPROJECT_STOCK_H
#define STOCKMARKETPROJECT_STOCK_H
#include <iostream>

class Stock
{
private:
    std::string stockId;
    float value_;
    uint64_t amount_;
public:
    const std::string &getStockId() const {
        return stockId;
    }

    void setStockId(const std::string &stockId) {
        Stock::stockId = stockId;
    }

    float getValue() const {
        return value_;
    }

    void setValue(float value) {
        Stock::value_ = value;
    }

    uint64_t getAmount() const {
        return amount_;
    }

    void setAmount(uint64_t amount) {
        Stock::amount_ = amount;
    }
};
#endif //STOCKMARKETPROJECT_STOCK_H
