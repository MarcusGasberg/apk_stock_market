//
// Created by stud on 11/28/20.
//

#ifndef STOCKMARKETPROJECT_SELLSTOCKCOMMAND_H
#define STOCKMARKETPROJECT_SELLSTOCKCOMMAND_H
#include <iostream>
#include <ostream>
#include "TransactionBase.h"

namespace stock
{
    class SellStockCommand : public TransactionBase
    {
        std::shared_ptr<TraderAccount<>> seller_account_;
        std::string stock_id_;
        std::shared_ptr<Stock> stock_;
        std::shared_ptr<Price> stock_price_;
    public:

        SellStockCommand() = default;


        explicit SellStockCommand(std::shared_ptr<TraderAccount<>> seller_account, std::string&& stock_id, std::shared_ptr<Price> stock_price)
            : seller_account_(std::move(seller_account)), stock_id_(std::move(stock_id)), stock_price_(std::move(stock_price))
        {
        }

        void execute()
        {
            stock_ = seller_account_->get_stock(stock_id_);
            seller_account_->sell_stock(*stock_, stock_price_->price_);
        }

        void undo()
        {
            if(stock_)
            {
                seller_account_->buy_stock(*stock_, stock_price_->price_);
            }
        }

        std::string get_description() const override
        {
            std::stringstream ss;
            ss << "SellCommand[" << get_id() << "]";
            return ss.str();
        }

        friend std::ostream& operator<<(std::ostream& os, const SellStockCommand& obj)
        {
            return os << obj.get_description();
        }
    };
}
#endif //STOCKMARKETPROJECT_SELLSTOCKCOMMAND_H
