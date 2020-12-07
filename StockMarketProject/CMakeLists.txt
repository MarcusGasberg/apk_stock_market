cmake_minimum_required(VERSION 3.17)
project(StockMarketProject)

set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.74.0 COMPONENTS REQUIRED) 


add_executable(StockMarketProject main.cpp headers/StockBroker.h headers/Commands/BuyStockCommand.h headers/Commands/SellStockCommand.h  headers/Commands/ListAllStockCommand.h   "headers/Exceptions/BadCommandException.h" "headers/CommandFactory.h" "headers/Commands/Command.h" "headers/Commands/CommandBase.h" "headers/Commands/UndoLatestCommand.h"  "headers/Queries/GetAllStockQuery.h")

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    target_link_libraries(StockMarketProject ${Boost_LIBRARIES})
endif()
 