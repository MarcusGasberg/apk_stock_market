//
// Created by stud on 11/30/20.
//

#ifndef STOCKMARKETPROJECT_STUDENTTRADERPOLICY_H
#define STOCKMARKETPROJECT_STUDENTTRADERPOLICY_H
#include "../AccountManager/AccountManager.h"
#include "../Helpers/IsNumeric.h"

class StudentTraderPolicy {
public:
    // TODO: Do check if amount is int, float, double...
    // Account Manager, Account Type
    template<typename T, typename = typename IsNumeric<T>::value>
    static void calculateCommission(AccountManager&& accountManager, T const& boughtAmount)
    {
        // 1.2% commission.
        accountManager.withdraw(boughtAmount * (100 + 1.2));
    }
};
#endif //STOCKMARKETPROJECT_STUDENTTRADERPOLICY_H
