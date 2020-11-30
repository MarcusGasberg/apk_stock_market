//
// Created by stud on 11/28/20.
//

#ifndef STOCKMARKETPROJECT_SELLSTOCKCOMMAND_H
#define STOCKMARKETPROJECT_SELLSTOCKCOMMAND_H
#include <iostream>

namespace stock
{
    struct SellStockCommand
    {
        static void execute()
        {
            std::cout << "Doing sell\n";
        }

        static void describe()
        {
            std::cout << "A sell command\n";
        }
    };
}
#endif //STOCKMARKETPROJECT_SELLSTOCKCOMMAND_H
