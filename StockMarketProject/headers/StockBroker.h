#pragma once
#include <variant>
#include <type_traits>
#include <tuple>
#include <vector>
#include <functional>

#include "Commands/BuyStockCommand.h"
#include "Commands/SellStockCommand.h"
#include "Commands/Command.h"

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

    template <typename List>
    class StockBroker
    {
        std::vector<std::function<void()>> undo_history;
        std::vector<typelist_variant_t<List>> all_commands;
    public:

        void handle_command(typelist_variant_t<List>& variant)
        {
            std::visit([&](auto&& command)
                {
                    using T = std::decay_t<decltype(command)>;
                    if constexpr (hasExecute<T>)
                    {
                        do_execute(command);
                    }
                    else
                    {
                        std::cout << "A command must implement execute" << std::endl;
                    }

                    if constexpr (hasUndo<T>)
                    {
                        store_undo(command);
                    }
                }, variant);
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

        template <typename Command>
        void store_undo(Command& command)
        {
            const std::function<void()> undo_f = std::bind(&Command::undo, command); // Could have done lambda instead
            undo_history.push_back(undo_f);
        }
    };
} // namespace stock
