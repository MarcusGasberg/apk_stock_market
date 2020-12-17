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
        std::shared_ptr<Stock> stock_;
        int stock_price_;
    public:

        BuyStockCommand()= default;

        BuyStockCommand(std::shared_ptr<TraderAccount<>> buyer_account) : buyer_account_(std::move(buyer_account))
        {
        }

        BuyStockCommand(std::shared_ptr<TraderAccount<>> buyer_account, std::shared_ptr<Stock> stock)
            : TransactionBase(),
            buyer_account_(std::move(buyer_account)), stock_(std::move(stock))
        {
        }


        [[nodiscard]] std::shared_ptr<Stock> stock() const
        {
            return stock_;
        }

        void set_stock(std::shared_ptr<Stock> stock)
        {
            stock_ = stock;
        }

        void execute()
        {
            if(stock_)
            {
                buyer_account_->BuyStock(*stock_, stock_price_);
            }
            else
            {
                std::cout << "Stock not found\n";
            }
        }

        void undo()
        {
            if (stock_)
            {
                buyer_account_->SellStock(*stock_, stock_price_);
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
