cmake_minimum_required(VERSION 3.17)
project(boost_signal_example)

set(CMAKE_CXX_STANDARD 17)

add_executable(boost_signal_example main.cpp GOF/Mediator.h GOF/Collegue.h headers/Helpers.h headers/Placeholders.h headers/StockMediator.h headers/TraderAccount.h headers/StockProvider.h headers/TraderTopics.h headers/TimeLiteral.h)