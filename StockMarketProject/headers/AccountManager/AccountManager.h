//
// Created by stud on 11/30/20.
//

#ifndef STOCKMARKETPROJECT_ACCOUNTMANAGER_H
#define STOCKMARKETPROJECT_ACCOUNTMANAGER_H

#include <iostream>

class AccountManager
{
private:
    float _funds;
public:
    AccountManager(float funds = 0) : _funds(funds) {};

    bool withdraw(float amount);
    bool deposit(float amount);
};

bool AccountManager::withdraw(float amount) {
    return false;
}

bool AccountManager::deposit(float amount) {
    return false;
}

#endif //STOCKMARKETPROJECT_ACCOUNTMANAGER_H
