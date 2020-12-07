//
// Created by stud on 11/30/20.
//

#ifndef STOCKMARKETPROJECT_STANDARDTRADERPOLICY_H
#define STOCKMARKETPROJECT_STANDARDTRADERPOLICY_H
#include "../AccountManager/AccountManager.h"
#include "../Helpers/IsNumeric.h"

class StandardTraderPolicy {
public:
    // TODO: Do check if amount is int, float, double...
    // Account Manager, Account Type
    template<typename T, typename = typename IsNumeric<T>::value>
    static void calculateCommission(AccountManager&& accountManager, T const& boughtAmount)
    {
        // 2% commission.
        accountManager.withdraw(boughtAmount * (100 + 2.0));
    }
};

#endif //STOCKMARKETPROJECT_STANDARDTRADERPOLICY_H
