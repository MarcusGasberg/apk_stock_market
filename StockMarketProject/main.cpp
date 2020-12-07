#include <iostream>
#include <variant>

#include "headers/CommandFactory.h"
#include "headers/StockBroker.h"
#include "headers/Commands/BuyStockCommand.h"
#include "headers/Commands/SellStockCommand.h"
#include "headers/Queries/GetAllStockQuery.h"

int main()
{
    std::tuple<> all_commands;


    using commands_t = stock::TypeList<stock::BuyStockCommand, stock::SellStockCommand, stock::ListAllStocksCommand>;
    using commands_var_t = stock::typelist_variant_t<commands_t>;

    using queries_t = stock::TypeList<stock::GetAllStockQuery>;
    using queries_var_t = stock::typelist_variant_t<queries_t>;

    std::cout << "_______________________________________" << std::endl;
    std::cout << "     Welcome To Northern Networks    " << std::endl;
    std::cout << "  Your Modern Stock Market Platform   " << std::endl;
    std::cout << "_______________________________________" << std::endl;
    std::cout << "1: Buy Stock" << std::endl;
    std::cout << "2: Sell Stock" << std::endl;
    std::cout << "3: List All Stocks" << std::endl;
    std::cout << "4: Undo Latest" << std::endl;

    stock::StockBroker<commands_t> stock_broker;

    boost::signals2::signal<void(commands_var_t&)> command_sig;
    boost::signals2::signal<void(queries_var_t&)> queries_sig;

    const std::function<void(queries_var_t&)> get_commands_from_stockbroker_f = [&stock_broker](queries_var_t& query)
    {
        std::visit([&stock_broker](auto&& q)
            {
                using T = std::decay_t<decltype(q)>;
                if constexpr (std::is_same_v<T, stock::GetAllStockQuery>)
                {
                    auto buy_stocks = stock_broker.get_all_commands_of_type<stock::BuyStockCommand>();
                    auto sell_stocks = stock_broker.get_all_commands_of_type<stock::SellStockCommand>();
                    for (const auto& b : buy_stocks)
                    {
                        q.result.push_back(b);
                    }
                    for (const auto& s : sell_stocks)
                    {
                        q.result.push_back(s);
                    }
                }
            }
        , query);
    };

    queries_sig.connect(get_commands_from_stockbroker_f);

    stock::CommandFactory<queries_var_t, commands_var_t> command_factory(queries_sig);


    while (true)
    {

        std::string line;
        std::getline(std::cin, line);

        const int choice = std::stoi(line);
        try
        {
            auto stock_command = command_factory.create_command(choice);

            stock_broker.handle_command(stock_command);
        }
        catch (const stock::BadCommandException& bad_command_exception)
        {
            std::cout << bad_command_exception.what() << std::endl;
        }
    }
}
