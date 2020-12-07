//
// Created by stud on 11/30/20.
//

#ifndef STOCKMARKETPROJECT_TRADERACCOUNT_H
#define STOCKMARKETPROJECT_TRADERACCOUNT_H
#include "StandardTraderPolicy.h"
#include "StudentTraderPolicy.h"

template<typename TraderPolicy> class TraderAccount;

// Perhaps use some type trait check on Account Manager?
template<typename TraderPolicy = StudentTraderPolicy>
class TraderAccount
{
    // Know all stock ids so it can send signal to StockMediator
    // Take some commission for trade based on behaviour policy (Student, Plus-Member, Standard)
    // Withdraw appropriated funds from account manager (either by reference or signal).

};
#endif //STOCKMARKETPROJECT_TRADERACCOUNT_H
