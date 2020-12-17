#pragma once
#include <iostream>
#include <string>
#include <boost/signals2/signal.hpp>
#include <utility>

#include "TransactionBase.h"

namespace stock
{
    class ListAllTransactionsCommand
    {
    public:

        ListAllTransactionsCommand() = default;

        ListAllTransactionsCommand(std::vector<std::shared_ptr<TransactionBase>> all_commands)
            : all_commands(std::move(all_commands))
        {
        }

        std::vector<std::shared_ptr<TransactionBase>> all_commands;

        void execute()
        {
            for (const auto& command : all_commands)
            {
                std::cout << command->get_description() << "\n";
            }
        }
    };
}
