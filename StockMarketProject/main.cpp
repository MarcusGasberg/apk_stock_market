#include <iostream>
#include <variant>

#include "headers/CommandBuilder.h"
#include "headers/StockBroker.h"
#include "headers/StockPrinter.h"
#include "headers/TransactionUndoer.h"
#include "headers/Commands/Commands.h"
#include "headers/Queries/Queries.h"
#include "headers/StockPrices/PriceProvider.h"
#include "headers/StockProviders/StockProvider.h"
#include "headers/Models/Price.h"
#include "headers/TraderAccount/TraderAccount.h"
#include "headers/StockPrices/StockPriceSimulator.h"

int main()
{

    std::cout << "_______________________________________" << std::endl;
    std::cout << "     Welcome To Northern Networks    " << std::endl;
    std::cout << "  Your Modern Stock Market Platform   " << std::endl;
    std::cout << "_______________________________________" << std::endl;
    std::cout << "0: Exit Program"  << std::endl;
    std::cout << "1: Buy Stock" << std::endl;
    std::cout << "2: Sell Stock" << std::endl;
    std::cout << "3: List All Transactions" << std::endl;
    std::cout << "4: Undo Latest" << std::endl;
    std::cout << "5: List All Available Stocks" << std::endl;

    boost::signals2::signal<void(std::shared_ptr<stock::commands_var_t>)> command_sig;
    boost::signals2::signal<void(std::shared_ptr<stock::queries_var_t>)> queries_sig;

    std::shared_ptr<stock::Mediator<void, stock::Stock&>> shared_mediator = std::make_shared<stock::Mediator<void, stock::Stock&>>();
    const auto my_account = std::make_shared<stock::TraderAccount<>>(stock::TraderAccount<>("Jens", shared_mediator, queries_sig));

    my_account->deposit(1000);

    stock::StockBroker stock_broker(queries_sig, command_sig);
    stock::StockPrinter stock_printer(command_sig);
    stock::CommandBuilder command_builder(queries_sig, my_account);

    stock::TransactionUndoer transaction_undoer(command_sig);

    std::shared_ptr<stock::PriceProvider> shared_price_provider = std::make_shared<stock::PriceProvider>();

    shared_price_provider.get()->add_stock("stock1", Price(22));
    shared_price_provider.get()->add_stock("stock2", Price(33));

    stock::StockProvider stock_provider("stock_provider", shared_price_provider, queries_sig, shared_mediator);

    stock_provider.add_stock_for_sale(stock::Stock("stock1", 10));
    stock_provider.add_stock_for_sale(stock::Stock("stock2", 20));

    stock::StockPriceSimulator sim;
    std::atomic<bool> termination_signal = false;
    auto t1 = sim.start_simulation(shared_price_provider, termination_signal);

    int choice = -1;
    do
    {

        std::string line;
        std::getline(std::cin, line);
        if(line.empty() || !std::all_of(line.begin(), line.end(), ::isdigit))
        {
            std::cout << "Please provide a number between 0 and 6\n";
            continue;
        }

        choice = std::stoi(line);

        try
        {
            if(choice == 0) {
                termination_signal = true;
                break;
            }

            const auto stock_command = command_builder.create_command(choice);
            if(stock_command)
            {
                command_sig(stock_command);
            }
        }
        catch (const stock::BadCommandException& bad_command_exception)
        {
            std::cout << bad_command_exception.what() << std::endl;
        }
    } while(choice != 0);

    t1.join();
    return 0;
}
