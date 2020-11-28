//
// Created by stud on 11/28/20.
//

#ifndef STOCKMARKETPROJECT_COMMAND_H
#define STOCKMARKETPROJECT_COMMAND_H

class StockTradeCommand {
public:
    virtual ~StockTradeCommand() {}
    virtual void Execute() const = 0; 
};
#endif //STOCKMARKETPROJECT_COMMAND_H
