//
// Created by stud on 12/17/20.
//

#ifndef STOCKMARKETPROJECT_PRICE_H
#define STOCKMARKETPROJECT_PRICE_H
#include "ctime"

class Price {
public:
    Price(int price): price_(price){
        time_t currentTime = time(0);
        dt = ctime(&currentTime);
    }
    char * dt;
    int price_;

};

#endif //STOCKMARKETPROJECT_PRICE_H
