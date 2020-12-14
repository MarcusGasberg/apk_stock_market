#pragma once
#include <variant>
#include <boost/signals2/signal.hpp>

#include "Commands/Command.h"
#include "Commands/ListAllStockCommand.h"
#include "Queries/GetAllTransactionsQuery.h"


namespace stock
{
    template <typename QueryVar, typename CommandVar>
    class StockPrinter
    {
        std::vector<std::shared_ptr<CommandVar>> all_commands;
        boost::signals2::signal<void(std::shared_ptr<QueryVar>)>& queries_sig_;
        boost::signals2::signal<void(std::shared_ptr<CommandVar>)>& command_sig_;

    public:
        StockPrinter(boost::signals2::signal<void(std::shared_ptr<QueryVar>)>& queries_sig,
                     boost::signals2::signal<void(std::shared_ptr<CommandVar>)>& command_sig)
            : queries_sig_(queries_sig),
              command_sig_(command_sig)
        {
            const std::function<void(std::shared_ptr<CommandVar>)> commands_f = [this](std::shared_ptr<CommandVar> variant)
            {
                std::visit([&](auto&& command)
                {
                    using T = std::decay_t<decltype(command)>;
                    if constexpr (!stock::hasExecute<T>)
                    {
                        return;
                    }
                    if constexpr (std::is_same_v <T, ListAllStocksCommand>)
                    {
                        handle(command);
                    }
                }, *variant);
            };


            command_sig_.connect(commands_f);
        }

        void handle(ListAllStocksCommand& command)
        {
            if(!command.all_commands.empty())
            {
                command.execute();
            }
        }
    };
}
