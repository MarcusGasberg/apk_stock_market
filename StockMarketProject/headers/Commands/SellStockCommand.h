#pragma once

#include <iostream>
#include <ostream>
#include "TransactionBase.h"

namespace stock
{
    class SellStockCommand : public TransactionBase
    {
        std::shared_ptr<TraderAccount<>> seller_account_;
        std::string stock_id_;
        std::shared_ptr<Price> stock_price_;
    public:

        SellStockCommand() = default;


        SellStockCommand(std::shared_ptr<TraderAccount<>> seller_account, std::string stock_id)
            : seller_account_(std::move(seller_account)), stock_id_(std::move(stock_id))
        {
        }

        bool execute()
        {
            return seller_account_->sell_stock(stock_id_);
        }
        
        bool undo()
        {
            return seller_account_->buy_stock(stock_id_);
        }

        std::string get_description() const override
        {
            std::stringstream ss;
            ss << "SellCommand[" << get_id() << "]: " << stock_id_;
            return ss.str();
        }

        friend std::ostream& operator<<(std::ostream& os, const SellStockCommand& obj)
        {
            return os << obj.get_description();
        }
    };
}