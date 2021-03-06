#pragma once
#include "TransactionManager.h"
#include "Commands/BuyStockCommand.h"
#include "Commands/ListAllStocksCommand.h"
#include "Commands/SellStockCommand.h"
#include "Exceptions/BadCommandException.h"
#include "boost/signals2/signal.hpp"
#include "Commands/TraderAccountDepositCommand.h"
#include "Helpers/Utility.h"
#include "Queries/GetAllStockQuery.h"
#include "Queries/GetAllTransactionsQuery.h"

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
                using TQuery = GetAllStockQuery;
                const std::shared_ptr<queries_var_t> queries_var = std::make_shared<queries_var_t>(TQuery());
                queries_sig_(queries_var);
                const auto queries_result = std::move(std::get<TQuery>(*queries_var));
                result = std::make_shared<commands_var_t>(ListAllStocksCommand(queries_result.result));
            }
            break;
            case 2:
                {
                    std::cout << "Please Provide the id of the stock you want to buy: ";
                    std::string stock_id;
                    std::getline(std::cin, stock_id);

                    result = std::make_shared<commands_var_t>(BuyStockCommand(trader_account_, stock_id));
                }
                break;
            case 3:
                {
                    std::cout << "Please Provide the id of the stock you want to sell: ";
                    std::string stock_id;
                    std::getline(std::cin, stock_id);
                   
                    result = std::make_shared<commands_var_t>(SellStockCommand(trader_account_, stock_id));
                }
                break;
            case 4:
                {
                    const std::shared_ptr<queries_var_t> queries_var = std::make_shared<queries_var_t>(GetAllTransactionsQuery());
                    queries_sig_(queries_var);
                    const GetAllTransactionsQuery queries_result = std::get<0>(*queries_var);
                    result = std::make_shared<commands_var_t>(ListAllTransactionsCommand(queries_result.get_result()));
                }
                break;
            case 5:
            {
                auto stocks = trader_account_->owned_stocks();
                for (auto&& stock : stocks)
                {
                    auto queries_var = std::make_shared<queries_var_t>(GetStockPriceQuery(stock.getStockId()));
                    queries_sig_(queries_var);
                    const GetStockPriceQuery price_result = std::move(std::get<GetStockPriceQuery>(*queries_var));

                    stock.setPrice(price_result.result);
                }
                result = std::make_shared<commands_var_t>(ListAllStocksCommand(stocks));
            }
            break;
            case 6:
                {
                    result = std::make_shared<commands_var_t>(UndoLatestCommand());
                }
                break;
            case 7:
                {
                    std::cout << "How much do you want to deposit: ";
                    std::string line;
                    std::getline(std::cin, line);
                    if (!utility::str_is_digit(line))
                    {
                        throw BadCommandException("Deposit must be a valid number");
                    }
                    const int amount = std::stoi(line);
                    result = std::make_shared<commands_var_t>(TraderAccountDepositAction(trader_account_, amount));
                }
                break;
            case 0:
                break;
            default:
                throw BadCommandException("Command not found");
            }

            return result;
        }
    };
}
