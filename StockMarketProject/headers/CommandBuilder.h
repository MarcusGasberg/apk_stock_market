#pragma once
#include "StockBroker.h"
#include "Commands/BuyStockCommand.h"
#include "Commands/ListAllStocksCommand.h"
#include "Commands/SellStockCommand.h"
#include "Exceptions/BadCommandException.h"
#include "boost/signals2/signal.hpp"
#include "Queries/GetAllStockQuery.h"
#include "Queries/GetAllTransactionsQuery.h"
#include "Queries/GetLatestStockQuery.h"
#include "Queries/GetStockQuery.h"
#include "Queries/GetTraderAccountQuery.h"

namespace stock
{
    class CommandBuilder
    {
        std::shared_ptr<TraderAccount<>> trader_account_;
        const boost::signals2::signal<void(std::shared_ptr<queries_var_t>)>& queries_sig_;
    public:
        explicit CommandBuilder(const boost::signals2::signal<void(std::shared_ptr<queries_var_t>)>& queries_sig, std::shared_ptr<TraderAccount<>> trader_account)
            : trader_account_(std::move(trader_account)), queries_sig_(queries_sig)
        {
        }


        std::shared_ptr<commands_var_t> create_command(int choice) const
        {
            std::shared_ptr<commands_var_t> result;
            switch (choice)
            {
            case 1:
            {
                std::cout << "What kind of stock do you want to buy?" << "\n";
                std::string stock_id;
                std::getline(std::cin, stock_id);

                const std::shared_ptr<queries_var_t> queries_var = std::make_shared<queries_var_t>(GetStockQuery(std::move(stock_id)));
                queries_sig_(queries_var);
                const GetStockQuery queries_result = std::get<GetStockQuery>(*queries_var);
                result = std::make_shared<commands_var_t>(BuyStockCommand(trader_account_, queries_result.result));
            }
            break;
            case 2:
                result = std::make_shared<commands_var_t>(SellStockCommand(trader_account_));
                break;
            case 3:
            {
                const std::shared_ptr<queries_var_t> queries_var = std::make_shared<queries_var_t>(GetAllTransactionsQuery());
                queries_sig_(queries_var);
                const GetAllTransactionsQuery queries_result = std::get<0>(*queries_var);
                result = std::make_shared<commands_var_t>(ListAllTransactionsCommand(queries_result.result));
            }
            break;
            case 4:
            {
                result = std::make_shared<commands_var_t>(UndoLatestCommand());
            }
            break;
            case 5:
            {
                using TQuery = GetAllStockQuery;
                const std::shared_ptr<queries_var_t> queries_var = std::make_shared<queries_var_t>(TQuery());
                queries_sig_(queries_var);
                const auto queries_result = std::get<TQuery>(*queries_var);
                result = std::make_shared<commands_var_t>(ListAllStocksCommand(queries_result.result));
            }
            break;
            default:
                throw BadCommandException("Command not found");
            }

            return result;
        }
    };
}
