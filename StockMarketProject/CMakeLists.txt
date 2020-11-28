cmake_minimum_required(VERSION 3.17)
project(StockMarketProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(StockMarketProject main.cpp headers/Commands/Command.h headers/Commands/BuyStockCommand.h headers/Commands/SellStockCommand.h)