#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "../Models/Stock.h"

namespace stock
{
    class ListAllStocksCommand
    {
    public:

        ListAllStocksCommand() = default;

        explicit ListAllStocksCommand(std::vector<Stock> all_stocks)
            : all_stocks_(std::move(all_stocks))
        {
        }

        std::vector<Stock> all_stocks_;


        void execute()
        {
            if(all_stocks_.empty())
            {
                std::cout << "No Stocks found\n";
                return;
            }
            for (const Stock& stock : all_stocks_)
            {
                std::cout << stock;
            }
        }
    };
}
