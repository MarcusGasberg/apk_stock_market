//
// Created by stud on 11/28/20.
//

#ifndef STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
#define STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "TransactionBase.h"
#include "../TraderAccount/TraderAccount.h"

namespace stock
{
    class BuyStockCommand : public TransactionBase
    {
        std::shared_ptr<TraderAccount<>> buyer_account_;
        std::string stock_id_;
    public:

        BuyStockCommand()= default;

        BuyStockCommand(std::shared_ptr<TraderAccount<>> buyer_account, std::string stock_id)
            : buyer_account_(std::move(buyer_account)), stock_id_(std::move(stock_id))
        {
        }

        bool execute() const
        {
            return buyer_account_->buy_stock(stock_id_);
        }

        bool undo() const
        {
            return buyer_account_->sell_stock(stock_id_);
        }

        std::string get_description() const override
        {
            std::stringstream ss;
            ss << "BuyCommand[" << get_id() << "]: " << stock_id_;
            return ss.str();
        }

        friend std::ostream& operator<<(std::ostream& os, const BuyStockCommand& obj)
        {
            return os << obj.get_description();
        }
    };
}
#endif //STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
