//
// Created by stud on 11/28/20.
//

#ifndef STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
#define STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
#include <iostream>
#include <sstream>
#include <string>

#include "CommandBase.h"

namespace stock
{
    class BuyStockCommand : public CommandBase
    {
    public:

        BuyStockCommand()
        {
            std::cout << "BUY CREATED\n";
        }

        void execute()
        {
            std::cout << "Doing buy\n";
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
