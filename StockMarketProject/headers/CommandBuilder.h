#pragma once
#include "StockBroker.h"
#include "Commands/BuyStockCommand.h"
#include "Commands/ListAllStockCommand.h"
#include "Commands/SellStockCommand.h"
#include "Exceptions/BadCommandException.h"
#include "boost/signals2/signal.hpp"
#include "Queries/GetAllTransactionsQuery.h"
#include "Queries/GetLatestStockQuery.h"
#include "Queries/GetStockQuery.h"
#include "Queries/GetTraderAccountQuery.h"

namespace stock
{
    template<typename QueriesVar, typename CommandsVar>
    class CommandBuilder
    {
        std::shared_ptr<TraderAccount<>> trader_account_;
        const boost::signals2::signal<void(std::shared_ptr<QueriesVar>)>& queries_sig_;
    public:
        explicit CommandBuilder(const boost::signals2::signal<void(std::shared_ptr<QueriesVar>)>& queries_sig, std::shared_ptr<TraderAccount<>> trader_account)
            : trader_account_(std::move(trader_account)), queries_sig_(queries_sig)
        {
        }


        std::shared_ptr<CommandsVar> create_command(int choice)
        {
            std::shared_ptr<CommandsVar> result;
            switch (choice)
            {
            case 1:
            {
                std::cout << "What kind of stock do you want to buy?" << "\n";
                std::string stock_id;
                std::getline(std::cin, stock_id);

                std::shared_ptr<QueriesVar> queries_var = std::make_shared<QueriesVar>(GetStockQuery(stock_id));
                queries_sig_(queries_var);
                const GetStockQuery queries_result = std::get<GetStockQuery>(*queries_var);
                result = std::make_shared<CommandsVar>(BuyStockCommand(trader_account_, queries_result.result));
            }
            break;
            case 2:
                result = std::make_shared<CommandsVar>(SellStockCommand(trader_account_));
                break;
            case 3:
            {
                std::shared_ptr<QueriesVar> queries_var = std::make_shared<QueriesVar>(GetAllTransactionsQuery());
                queries_sig_(queries_var);
                const GetAllTransactionsQuery queries_result = std::get<0>(*queries_var);
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
