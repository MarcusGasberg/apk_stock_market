#pragma once
#include "StockBroker.h"
#include "Commands/BuyStockCommand.h"
#include "Commands/ListAllStockCommand.h"
#include "Commands/SellStockCommand.h"
#include "Exceptions/BadCommandException.h"
#include "boost/signals2/signal.hpp"
#include "Queries/GetAllStockQuery.h"

namespace stock
{
    template<typename QueriesVar, typename CommandsVar>
    class CommandFactory
    {
        const boost::signals2::signal<void(QueriesVar&)>& queries_sig_;
    public:
        explicit CommandFactory(const boost::signals2::signal<void(QueriesVar&)>& queries_sig)
            : queries_sig_(queries_sig)
        {
        }

        CommandsVar create_command(int choice)
        {
            CommandsVar result;
            switch (choice)
            {
            case 1:
                result = BuyStockCommand();
                break;
            case 2:
                result = SellStockCommand();
                break;
            case 3:
                {
                    ListAllStocksCommand command_result;
                    QueriesVar queries_var = GetAllStockQuery();
                    queries_sig_(queries_var);
                    const GetAllStockQuery queries_result = std::get<0>(queries_var);
                    command_result.all_commands = queries_result.result;
                    result = command_result;
                }
                break;
            default:
                throw BadCommandException("Command not found");
            }

            return result;
        }
    };
}
