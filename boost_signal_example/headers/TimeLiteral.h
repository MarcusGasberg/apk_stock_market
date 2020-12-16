//
// Created by stud on 12/16/20.
//

#ifndef BOOST_SIGNAL_EXAMPLE_TIMELITERAL_H
#define BOOST_SIGNAL_EXAMPLE_TIMELITERAL_H
#include <iostream>

// Inspiration from https://en.cppreference.com/w/cpp/language/user_literal
// used for side-effects
void operator"" _print(const char * str, std::size_t N)
{
    time_t currentTime = time(0);
    char * dt = ctime(&currentTime);
    std::cout << std::string{str} << " " << dt << std::endl;
}
#endif //BOOST_SIGNAL_EXAMPLE_TIMELITERAL_H
