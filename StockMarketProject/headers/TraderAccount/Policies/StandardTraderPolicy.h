//
// Created by stud on 11/30/20.
//

#ifndef STOCKMARKETPROJECT_STANDARDTRADERPOLICY_H
#define STOCKMARKETPROJECT_STANDARDTRADERPOLICY_H
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
        template<typename T, typename = IsNumeric<T>>
        static T calculate_commission(T boughtAmount, T stockPrice) {
            return (stockPrice * boughtAmount * (commissionRate / 100));
        }
    };
}

#endif //STOCKMARKETPROJECT_STANDARDTRADERPOLICY_H
