#include <iostream>
#include <sstream>
#include <variant>

#include "headers/StockBroker.h"
#include "headers/Commands/BuyStockCommand.h"
#include "headers/Commands/SellStockCommand.h"

int main()
{
    std::tuple<> all_commands;


    using commands = stock::TypeList<stock::BuyStockCommand, stock::SellStockCommand, stock::ListAllStocksCommand>;

    std::cout << "_______________________________________" << std::endl;
    std::cout << "     Welcome To Northern Networks    " << std::endl;
    std::cout << "  Your Modern Stock Market Platform   " << std::endl;
    std::cout << "_______________________________________" << std::endl;
    std::cout << "1: Buy Stock" << std::endl;
    std::cout << "2: Sell Stock" << std::endl;
    std::cout << "3: List All Stocks" << std::endl;
    while (true)
    {

        std::string line;
        std::getline(std::cin, line);

        const int choice = std::stoi(line);
        try
        {
            const auto stock_command = stock::create_command<commands>(choice);

            stock::handle_command<commands>(stock_command);
        }
        catch (const stock::BadCommandException& bad_command_exception)
        {
            std::cout << bad_command_exception.what() << std::endl;
        }
    }

    
}
