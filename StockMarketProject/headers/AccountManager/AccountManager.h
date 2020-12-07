//
// Created by stud on 11/30/20.
//

#ifndef STOCKMARKETPROJECT_ACCOUNTMANAGER_H
#define STOCKMARKETPROJECT_ACCOUNTMANAGER_H
class AccountManager
{
private:
    float _funds;
public:
    AccountManager(float funds = 0);

    bool withdraw(float amount);
    bool deposit(float amount);
};

#endif //STOCKMARKETPROJECT_ACCOUNTMANAGER_H
