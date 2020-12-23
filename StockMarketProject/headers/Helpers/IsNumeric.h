//
// Created by stud on 11/30/20.
//

#ifndef STOCKMARKETPROJECT_ISNUMERIC_H
#define STOCKMARKETPROJECT_ISNUMERIC_H
#include <iostream>

namespace stock::utility {
    template<class T>
    struct IsNumeric {
        static constexpr bool value =
                std::is_floating_point<T>::value ||
                std::is_integral<T>::value;
    };
}

#endif //STOCKMARKETPROJECT_ISNUMERIC_H
