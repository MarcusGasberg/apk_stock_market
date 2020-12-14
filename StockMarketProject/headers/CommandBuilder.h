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

        const boost::signals2::signal<void(std::shared_ptr<QueriesVar>)>& queries_sig_;
    public:
        explicit CommandBuilder(const boost::signals2::signal<void(std::shared_ptr<QueriesVar>)>& queries_sig)
            : queries_sig_(queries_sig)
        {
        }

        std::shared_ptr<CommandsVar> create_command(int choice)
        {
            std::shared_ptr<CommandsVar> result;
            switch (choice)
            {
            case 1:
                result = std::make_shared<CommandsVar>(BuyStockCommand());
                break;
            case 2:
                result = std::make_shared<CommandsVar>(SellStockCommand());
                break;
            case 3:
                {
                    std::shared_ptr<QueriesVar> queries_var = std::make_shared<QueriesVar>(GetAllStockQuery());
                    queries_sig_(queries_var);
                    const GetAllStockQuery queries_result = std::get<0>(*queries_var);
                    result = std::make_shared<CommandsVar>(ListAllStocksCommand(queries_result.result));
                }
                break;
            case 4:
                {
                    result = std::make_shared<CommandsVar>(UndoLatestCommand());
                }
                break;
            default:
                throw BadCommandException("Command not found");
            }

            return result;
        }
    };
}
