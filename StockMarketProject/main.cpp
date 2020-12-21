#include <iostream>
#include "headers/CommandBuilder.h"
#include "headers/TransactionManager.h"
#include "headers/StockPrinter.h"
#include "headers/TransactionUndoer.h"
#include "headers/Helpers/Utility.h"
#include "headers/AccountManager/AccountManager.h"
#include "headers/Commands/Commands.h"
#include "headers/Queries/Queries.h"
#include "headers/StockPrices/PriceProvider.h"
#include "headers/StockBrokers//StockBroker.h"
#include "headers/Models/Price.h"
#include "headers/TraderAccount/TraderAccount.h"
#include "headers/StockPrices/StockPriceSimulator.h"

int main()
{
    std::cout << "_______________________________________\n";
    std::cout << "     Welcome To Northern Networks    \n";
    std::cout << "  Your Modern Stock Market Platform   \n";
    std::cout << "_______________________________________\n";
    std::cout << "0: Exit Program\n";
    std::cout << "1: Buy Stock\n";
    std::cout << "2: Sell Stock\n";
    std::cout << "3: List All Transactions\n";
    std::cout << "4: Undo Latest\n";
    std::cout << "5: List All Available Stocks\n";
    std::cout << "6: List Owned Stocks\n";
    std::cout << "7: Deposit Funds\n\n";

    stock::commands_sig_t command_sig;
    stock::queries_sig_t queries_sig;

    std::shared_ptr<stock::Mediator<void, stock::Stock&>> shared_mediator = std::make_shared<stock::Mediator<void, stock::Stock&>>();
    const auto my_account = std::make_shared<stock::TraderAccount<>>(stock::TraderAccount<>("Jens", shared_mediator, queries_sig));

    my_account->deposit(1000);

    stock::AccountManager account_manager(command_sig);
    stock::TransactionManager stock_broker(queries_sig, command_sig);
    stock::StockPrinter stock_printer(command_sig);
    stock::CommandBuilder command_builder(queries_sig, my_account);

    stock::TransactionUndoer transaction_undoer(command_sig);
    std::shared_ptr<stock::PriceProvider> shared_price_provider = std::make_shared<stock::PriceProvider>();
    shared_price_provider->add_stock("Novo Nordisk", Price(11));
    shared_price_provider->add_stock("Maersk", Price(22));
    shared_price_provider->add_stock("British Airways", Price(33));
    shared_price_provider->add_stock("British American Tobacco", Price(33));

    stock::StockBroker stock_provider_london("London Stock Exchange", shared_price_provider, queries_sig, shared_mediator);
    stock::StockBroker stock_provider_nasdaq("NASDAQ", shared_price_provider, queries_sig, shared_mediator);

    stock_provider_nasdaq.add_stock_for_sale(stock::Stock("Novo Nordisk", 10, "NASDAQ"));
    stock_provider_nasdaq.add_stock_for_sale(stock::Stock("Maersk", 20, "NASDAQ"));
    stock_provider_london.add_stock_for_sale(stock::Stock("British Airways", 30, "London Stock Exchange"));
    stock_provider_london.add_stock_for_sale(stock::Stock("British American Tobacco", 405, "London Stock Exchange"));

    stock::StockPriceSimulator sim;
    std::atomic<bool> termination_signal = false;
    auto t1 = sim.start_simulation(shared_price_provider, termination_signal);

    int choice = -1;
    do
    {

        std::string line;
        std::getline(std::cin, line);
        if(!str_is_digit(line))
        {
            std::cout << "Please provide a number between 0 and 7\n";
            continue;
        }

        choice = std::stoi(line);

        try
        {
            if(choice == 0) {
                //termination_signal = true;
                break;
            }

            const auto stock_command = command_builder.create_command(choice);
            if(stock_command)
            {
                command_sig(stock_command);
            }
        }
        catch (const stock::NoPriceException& exception)
        {
            std::cout << exception.what() << std::endl;
        }
        catch (const stock::BadCommandException& exception)
        {
            std::cout << exception.what() << std::endl;
        }
    } while(choice != 0);

    t1.join();
    return 0;
}
