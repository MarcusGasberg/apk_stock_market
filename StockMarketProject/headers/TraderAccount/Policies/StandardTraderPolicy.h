//
// Created by stud on 11/30/20.
//

#ifndef STOCKMARKETPROJECT_STANDARDTRADERPOLICY_H
#define STOCKMARKETPROJECT_STANDARDTRADERPOLICY_H
#include "../../AccountManager/AccountManager.h"
#include "../../Helpers/IsNumeric.h"

namespace stock {
    /*
    * Normal commission rate
    * Normal tax regulations
    * Country commission rate
    */
    class StandardTraderPolicy {
    private:
        static constexpr float commissionRate = 2.0;
    public:
        // Account Manager, Account Type
        template<typename T, typename = typename IsNumeric<T>::value>
        static void takeCommission(AccountManager &&accountManager, T const &boughtAmount) {
            accountManager.withdraw(boughtAmount * commissionRate);
        }
    };
}

#endif //STOCKMARKETPROJECT_STANDARDTRADERPOLICY_H
