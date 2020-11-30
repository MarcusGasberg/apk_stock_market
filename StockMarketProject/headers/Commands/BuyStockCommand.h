//
// Created by stud on 11/28/20.
//

#ifndef STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
#define STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
#include <iostream>
#include <string>

namespace stock
{
    struct BuyStockCommand
    {
        static void execute()
        {
            std::cout << "Doing buy " << "\n";
        }

        static void describe()
        {
            std::cout << "A buy command\n";
        }
    };
}
#endif //STOCKMARKETPROJECT_BUYSTOCKCOMMAND_H
