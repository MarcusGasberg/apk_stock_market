//
// Created by stud on 11/28/20.
//

#ifndef STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
#define STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
#include "Command.h"

class BuyStockCommand : StockTradeCommand
{
public:
    void Execute() const override {
        // Buy logic...
    }
};
#endif //STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
