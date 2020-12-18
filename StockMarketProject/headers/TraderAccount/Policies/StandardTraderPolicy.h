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
        static constexpr float commission_rate = 2.0;
        static constexpr float flat_rate = 30;
    public:
        template<typename T, typename = IsNumeric<T>>
        static T calculate_commission(T boughtAmount, T stockPrice) {
            int commission = (stockPrice * boughtAmount * (commission_rate / 100));
            if(commission > 30) return commission;
            else return flat_rate;
        }
    };
}

#endif //STOCKMARKETPROJECT_STANDARDTRADERPOLICY_H
