#pragma once

#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

#include "../TraderAccount/TraderAccount.h"

namespace stock
{
    class TraderAccountDepositAction
    {
        std::shared_ptr<TraderAccount<>> trader_account_;
        int amount_;
    public:

        TraderAccountDepositAction() = default;

        TraderAccountDepositAction(std::shared_ptr<TraderAccount<>> trader_account, int amount): trader_account_(std::move(trader_account)), amount_(amount)
        {
        }

        void execute() const
        {
            std::cout << "========= " << "Deposit" << " =========" << "\n";
            if(trader_account_ && amount_ > 0)
            {
                trader_account_->deposit(amount_);
                std::cout << "Your new balance is: " << trader_account_->get_balance() << "\n";
                {
                }
            }
        }
    };
}
