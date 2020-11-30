#pragma once
#include <iostream>
#include <string>

namespace stock
{
    class ListAllStocksCommand
    {
    public:

        using ResultType = std::string;

        static void describe()
        {
            std::cout << "A buy command\n";
        }
    };
}
