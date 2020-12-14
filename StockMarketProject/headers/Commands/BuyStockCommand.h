//
// Created by stud on 11/28/20.
//

#ifndef STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
#define STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "CommandBase.h"
#include "../TraderAccount/TraderAccount.h"

namespace stock
{
    class BuyStockCommand : public CommandBase
    {
        std::shared_ptr<TraderAccount<>> buyer_account_;
        std::shared_ptr<Stock> stock_;
    public:

        BuyStockCommand()= default;


        BuyStockCommand(std::shared_ptr<TraderAccount<>> buyer_account, std::shared_ptr<Stock> stock)
            :
              buyer_account_(std::move(buyer_account)), stock_(std::move(stock))
        {
        }

        void execute()
        {
            buyer_account_->BuyStock(*stock_);
        }

        void undo()
        {
            std::cout << get_description() << ": Undoing buy...\n";
        }

        [[nodiscard]] std::string get_description() const override
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
