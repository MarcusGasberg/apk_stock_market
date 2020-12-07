//
// Created by stud on 11/30/20.
//

#ifndef STOCKMARKETPROJECT_TRADERACCOUNT_H
#define STOCKMARKETPROJECT_TRADERACCOUNT_H
#include <vector>

#include "StandardTraderPolicy.h"
#include "StudentTraderPolicy.h"
#include "../StockMediator/Stock.h"

template<typename TraderPolicy> class TraderAccount;

// Know all stock ids so it can send signal to StockMediator
// Take some commission for trade based on behaviour policy (Student, Plus-Member, Standard)
// Withdraw appropriated funds from account manager (either by reference or signal).
template<typename TraderPolicy = StandardTraderPolicy>
class TraderAccount
{
private:
    std::vector<Stock> ownedStocks;
public:
    // TODO: Perfect forwarding?
    bool BuyStock(Stock & stock);
    bool SellStock(Stock & stock);
};

template<typename TraderPolicy>
bool TraderAccount<TraderPolicy>::BuyStock(Stock & stock) {
    return true;
}

template<typename TraderPolicy>
bool TraderAccount<TraderPolicy>::SellStock(Stock & stock) {
    return true;
}

#endif //STOCKMARKETPROJECT_TRADERACCOUNT_H
