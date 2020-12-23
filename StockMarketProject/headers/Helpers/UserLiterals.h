//
// Created by stud on 12/16/20.
//

#ifndef STOCKMARKETPROJECT_USERLITERALS_H
#define STOCKMARKETPROJECT_USERLITERALS_H
#include <ctime>
#include <iostream>

namespace stock::utility {
// Inspiration from https://en.cppreference.com/w/cpp/language/user_literal
// used for side-effects
    inline void operator "" _print(const char *str, std::size_t N) {
        time_t currentTime = time(0);
        char *dt = ctime(&currentTime);
        std::cout << std::string{str} << " " << dt << std::endl;
    }
}

#endif //STOCKMARKETPROJECT_USERLITERALS_H
