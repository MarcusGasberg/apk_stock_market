#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "../Models/Stock.h"

namespace stock
{
    class ListAllStocksCommand
    {
    public:

        ListAllStocksCommand() = default;

        explicit ListAllStocksCommand(const std::vector<Stock>& all_stocks)
            : all_stocks_(all_stocks)
        {
        }

        std::vector<Stock> all_stocks_;


        void execute()
        {
            for (const Stock& stock : all_stocks_)
            {
                std::cout << "ID: "<< stock.getStockId()<< ", Amount: " << stock.getAmount() << "\n";
            }
        }
    };
}
