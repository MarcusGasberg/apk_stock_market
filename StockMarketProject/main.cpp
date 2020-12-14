#include <iostream>
#include <variant>

#include "headers/CommandBuilder.h"
#include "headers/StockBroker.h"
#include "headers/StockPrinter.h"
#include "headers/Commands/BuyStockCommand.h"
#include "headers/Commands/SellStockCommand.h"
#include "headers/Queries/GetAllStockQuery.h"

int main()
{
    using commands_t = stock::TypeList<stock::BuyStockCommand, stock::SellStockCommand, stock::ListAllStocksCommand, stock::UndoLatestCommand>;
    using commands_var_t = stock::typelist_variant_t<commands_t>;

    using queries_t = stock::TypeList<stock::GetAllStockQuery, stock::GetLatestStockQuery>;
    using queries_var_t = stock::typelist_variant_t<queries_t>;

    std::cout << "_______________________________________" << std::endl;
    std::cout << "     Welcome To Northern Networks    " << std::endl;
    std::cout << "  Your Modern Stock Market Platform   " << std::endl;
    std::cout << "_______________________________________" << std::endl;
    std::cout << "1: Buy Stock" << std::endl;
    std::cout << "2: Sell Stock" << std::endl;
    std::cout << "3: List All Stocks" << std::endl;
    std::cout << "4: Undo Latest" << std::endl;


    boost::signals2::signal<void(std::shared_ptr<commands_var_t>)> command_sig;
    boost::signals2::signal<void(std::shared_ptr<queries_var_t>)> queries_sig;

    stock::StockBroker<queries_var_t, commands_var_t> stock_broker(queries_sig, command_sig);
    stock::StockPrinter<queries_var_t, commands_var_t> stock_printer(queries_sig, command_sig);
    stock::CommandBuilder<queries_var_t, commands_var_t> command_builder(queries_sig);


    while (true)
    {

        std::string line;
        std::getline(std::cin, line);
        const int choice = std::stoi(line);

        try
        {
            const auto stock_command = command_builder.create_command(choice);
            command_sig(stock_command);
        }
        catch (const stock::BadCommandException& bad_command_exception)
        {
            std::cout << bad_command_exception.what() << std::endl;
        }
    }
}
