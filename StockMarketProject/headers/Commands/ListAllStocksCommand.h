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
            std::cout << "========= " << "List All Stocks" << " =========" "\n";
            if(all_stocks_.empty())
            {
                std::cout << "No Stocks found\n";
                return;
            }

            const std::ostream_iterator<Stock> ostream_iterator(std::cout);
            std::copy(all_stocks_.begin(), all_stocks_.end(), ostream_iterator);
        }
    };
}
