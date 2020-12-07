cmake_minimum_required(VERSION 3.17)
project(StockMarketProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(StockMarketProject main.cpp headers/Commands/Command.h headers/Commands/BuyStockCommand.h headers/Commands/SellStockCommand.h headers/TraderAccount/TraderAccount.h headers/TraderAccount/StandardTraderPolicy.h headers/AccountManager/AccountManager.h source/AccountManager.cpp headers/TraderAccount/StudentTraderPolicy.h headers/Helpers/IsNumeric.h headers/Mixins/Interface/Bells.h headers/Mixins/Implementation/Guts.h headers/Mixins/Abilities/AbilityTags.h headers/Mixins/Compose.h)