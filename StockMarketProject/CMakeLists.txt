cmake_minimum_required(VERSION 3.17)
project(StockMarketProject)

set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.71.0 COMPONENTS REQUIRED)

add_executable(StockMarketProject main.cpp headers/StockBroker.h headers/Commands/BuyStockCommand.h headers/Commands/SellStockCommand.h  headers/Commands/ListAllTransactionsCommand.h   "headers/Exceptions/BadCommandException.h"  "headers/Commands/Commands.h" "headers/Commands/TransactionBase.h" "headers/Commands/UndoLatestCommand.h"  "headers/Queries/GetAllTransactionsQuery.h"  "headers/StockPrinter.h" "headers/StockMediator/Mediator.h" "headers/Models/Stock.h" "headers/Queries/GetStockQuery.h" headers/StockMediator/Placeholders.h headers/TraderAccount/TraderTopics.h "headers/TraderAccount/TraderAccount.h" headers/StockProviders/StockProvider.h headers/Helpers/UserLiterals.h "headers/TransactionUndoer.h" "headers/Queries/Queries.h" headers/StockPrices/PriceProvider.h  "headers/Commands/ListAllStocksCommand.h" headers/Models/Price.h "headers/Queries/GetStockPriceQuery.h"  "headers/Commands/TraderAccountDepositCommand.h" "headers/Helpers/Utility.h" "headers/StockPrices/StockPriceSimulator.h")


if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(StockMarketProject ${Boost_LIBRARIES})
endif()