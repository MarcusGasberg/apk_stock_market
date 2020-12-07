#pragma once
#include <iostream>
#include <string>
#include <boost/signals2/signal.hpp>

namespace stock
{
    class ListAllStocksCommand
    {
    public:

        ListAllStocksCommand() = default;

        ListAllStocksCommand(const std::vector<std::shared_ptr<CommandBase>>& all_commands)
            : all_commands(all_commands)
        {
        }

        std::vector<std::shared_ptr<CommandBase>> all_commands;

        void execute()
        {
            for (const auto& all_command : all_commands)
            {
                std::cout << all_command->get_description() << "\n";
            }
        }
    };
}