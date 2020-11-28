//
// Created by stud on 11/28/20.
//

#ifndef STOCKMARKETPROJECT_SELLSTOCKCOMMAND_H
#define STOCKMARKETPROJECT_SELLSTOCKCOMMAND_H
#include "Command.h"

class SellStockCommand : StockTradeCommand
{
public:
    void Execute() const override {
        // Sell logic...
    }
};
#endif //STOCKMARKETPROJECT_SELLSTOCKCOMMAND_H
