//
// Created by stud on 12/14/20.
//

#ifndef BOOST_SIGNAL_EXAMPLE_TRADERACCOUNT_H
#define BOOST_SIGNAL_EXAMPLE_TRADERACCOUNT_H

#include "StockMediator.h"
#include <string>


template<typename returnType, typename... params>
class TraderAccount {
    std::string buy_;
    std::string sell_;
public:
    StockMediator<returnType, params...> mediator_;
    TraderAccount(StockMediator<returnType, params...> && mediator) : mediator_(std::move(mediator)), buy_("buy"), sell_("sell"){}

    void buy()
    {
        // Do business logic
        // Trigger mediator topic "buy"

        mediator_.notify(buy_, 10, 22);
    }

    void sell()
    {
        // Do business logic
        // Trigger mediator topic "sell"
        mediator_.notify(sell_, 10, 22);
    }
};
#endif //BOOST_SIGNAL_EXAMPLE_TRADERACCOUNT_H
