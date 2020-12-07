#pragma once
#include <variant>
#include <type_traits>
#include <tuple>
#include <vector>
#include <functional>

#include "Commands/BuyStockCommand.h"
#include "Commands/SellStockCommand.h"
#include "Commands/Command.h"
#include "Exceptions/BadCommandException.h"

namespace stock
{
    template <typename... T>
    struct TypeList
    {
        template<template<typename ...> typename MFn>
        using apply = MFn<T...>;
    };

    template <template<typename ...> typename MFn, typename TList >
    using apply = typename TList::template apply<MFn>;


    template <typename TList>
    using typelist_variant_t = apply<std::variant, TList>;

    template <typename ...T>
    std::variant<T...> as_variant(TypeList<T...>);


    template <typename ...T>
    std::tuple<T...> as_tuple(TypeList<T...>);

    template <typename... Command>
    void list_trades(Command &&... args)
    {
        (args.describe(), ...);
    }

    template<typename T>
    class to_vector_visitor
    {
        std::vector<std::shared_ptr<T>>& vec_;
    public:
        explicit to_vector_visitor(std::vector<std::shared_ptr<T>>& vec)
            : vec_(vec)
        {
        }

        void operator ()(T& val)
        {
            vec_.push_back(std::make_shared<T>(val));
        }

        void operator()(...) const {}
    };

    template <typename CommandVar>
    class StockBroker
    {
        std::vector<CommandVar> all_commands;
    public:
        template<typename Command>
        void handle_command(Command& command)
        {
            using T = std::decay_t<decltype(command)>;
            if constexpr (!hasUndo<T>)
            {
                throw stock::BadCommandException();
            }
            if constexpr (std::is_same_v<T, BuyStockCommand>)
            {
                buy(command);
            }
            if constexpr (std::is_same_v<T, SellStockCommand>)
            {
                sell(command);
            }
        }

        void buy(BuyStockCommand& buy_command)
        {
            std::cout << "Stockbroker doing buy...\n";
            buy_command.execute();
            all_commands.push_back(buy_command);

        }

        void sell(SellStockCommand& sell_command)
        {
            std::cout << "Stockbroker doing sell...\n";
            sell_command.execute();
            all_commands.push_back(sell_command);
        }

        template<typename Command>
        std::vector<std::shared_ptr<Command>> get_all_commands_of_type()
        {
            std::vector<std::shared_ptr<Command>> commands;
            to_vector_visitor<Command> visitor(commands);
            for (size_t i = 0; i < all_commands.size(); ++i)
            {
                std::visit(visitor, all_commands[i]);
            }
            return commands;
        }

        template <typename Command>
        void do_execute(Command& command)
        {
            command.execute();
            all_commands.push_back(command);
        }
    };
} // namespace stock
