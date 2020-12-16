#pragma once
#include <iostream>
#include <string>
#include <boost/signals2/signal.hpp>
#include <utility>

namespace stock
{
    class ListAllStocksCommand
    {
    public:

        ListAllStocksCommand() = default;

        ListAllStocksCommand(std::vector<std::shared_ptr<TransactionBase>> all_commands)
            : all_commands(std::move(all_commands))
        {
        }

        std::vector<std::shared_ptr<TransactionBase>> all_commands;

        void execute()
        {
            for (const auto& all_command : all_commands)
            {
                std::cout << all_command->get_description() << "\n";
            }
        }
    };
}
