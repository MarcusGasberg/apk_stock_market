//
// Created by stud on 11/28/20.
//

#ifndef STOCKMARKETPROJECT_SELLSTOCKCOMMAND_H
#define STOCKMARKETPROJECT_SELLSTOCKCOMMAND_H
#include <iostream>
#include <ostream>
#include "CommandBase.h"

namespace stock
{
    class SellStockCommand : public CommandBase
    {
    public:
        SellStockCommand() = default;

        void execute()
        {
            std::cout << "Doing sell\n";
        }

        void undo()
        {
            std::cout << "Undoing sell..." << "\n";
        }

        [[nodiscard]] std::string get_description() const override
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
