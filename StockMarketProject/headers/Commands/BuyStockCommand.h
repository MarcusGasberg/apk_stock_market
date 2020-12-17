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
        std::shared_ptr<Price> price_;
        std::string stock_id_;
    public:

        BuyStockCommand()= default;

        BuyStockCommand(std::shared_ptr<TraderAccount<>> buyer_account, std::string stock_id, std::shared_ptr<Price> price)
            : TransactionBase(),
            buyer_account_(std::move(buyer_account)), price_(std::move(price)), stock_id_(std::move(stock_id))
        {
        }

        void execute()
        {
            if(price_)
            {
                buyer_account_->buy_stock(stock_id_, price_->price_);
            }
            else
            {
                std::cout << "Stock not found\n";
            }
        }

        void undo()
        {
            if (price_)
            {
                buyer_account_->sell_stock(stock_id_, price_->price_);
            }
        }

        std::string get_description() const override
        {
            std::stringstream ss;
            ss << "BuyCommand[" << get_id() << "]";
            return ss.str();
        }

        friend std::ostream& operator<<(std::ostream& os, const BuyStockCommand& obj)
        {
            return os << obj.get_description();
        }
    };
}
#endif //STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
