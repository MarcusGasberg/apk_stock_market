//
// Created by stud on 11/30/20.
//

#ifndef STOCKMARKETPROJECT_STUDENTTRADERPOLICY_H
#define STOCKMARKETPROJECT_STUDENTTRADERPOLICY_H
#include "../../AccountManager/AccountManager.h"
#include "../../Helpers/IsNumeric.h"


namespace stock {
    /*
    * Lower commission rate
    * Different tax regulations
    * No country commission rate
    */
    class StudentTraderPolicy {
    private:
        static constexpr float commissionRate = 1.2;
    public:
        // Account Manager, Account Type
        template<typename T, typename = typename IsNumeric<T>::value>
        static void takeCommission(AccountManager &&accountManager, T const &boughtAmount) {
            accountManager.withdraw(boughtAmount * commissionRate);
        }
    };
}
#endif //STOCKMARKETPROJECT_STUDENTTRADERPOLICY_H
