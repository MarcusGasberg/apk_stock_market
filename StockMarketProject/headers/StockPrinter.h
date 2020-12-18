#pragma once
#include <variant>
#include <boost/signals2/signal.hpp>

#include "Commands/ListAllStocksCommand.h"
#include "Commands/ListAllTransactionsCommand.h"
#include "Queries/GetAllTransactionsQuery.h"


namespace stock
{
    class StockPrinter
    {
        std::vector<std::shared_ptr<commands_var_t>> all_commands;

    public:
        StockPrinter(boost::signals2::signal<void(std::shared_ptr<commands_var_t>)>& command_sig)
        {
            const std::function<void(std::shared_ptr<commands_var_t>)> commands_f = [this](std::shared_ptr<commands_var_t> variant)
            {
                std::visit([&](auto&& command)
                {
                    using T = std::decay_t<decltype(command)>;
                    if constexpr (!stock::hasExecute<T>)
                    {
                        return;
                    }
                    if constexpr (std::is_same_v <T, ListAllTransactionsCommand> || std::is_same_v <T, ListAllStocksCommand>)
                    {
                        handle(command);
                    }
                }, *variant);
            };


            command_sig.connect(commands_f);
        }

        template<typename Command>
        void handle(Command& command)
        {
            command.execute();
        }
    };
}
