#pragma once
#include "StockBroker.h"
#include "Commands/BuyStockCommand.h"
#include "Commands/ListAllStockCommand.h"
#include "Commands/SellStockCommand.h"
#include "Exceptions/BadCommandException.h"
#include "boost/signals2/signal.hpp"
#include "Queries/GetAllStockQuery.h"
#include "Queries/GetLatestStockQuery.h"

namespace stock
{
    template<typename QueriesVar, typename CommandsVar>
    class CommandBuilder
    {

        const boost::signals2::signal<void(QueriesVar&)>& queries_sig_;
    public:
        explicit CommandBuilder(const boost::signals2::signal<void(QueriesVar&)>& queries_sig)
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
                    QueriesVar queries_var = GetAllStockQuery();
                    queries_sig_(queries_var);
                    const GetAllStockQuery queries_result = std::get<0>(queries_var);
                    result = ListAllStocksCommand(queries_result.result);
                }
                break;
            case 4:
                {
                    QueriesVar queries_var = GetLatestStockQuery();
                    queries_sig_(queries_var);
                    const GetAllStockQuery queries_result = std::get<0>(queries_var);
                    result = ListAllStocksCommand(queries_result.result);
                }
                break;
            default:
                throw BadCommandException("Command not found");
            }

            return result;
        }
    };
}
