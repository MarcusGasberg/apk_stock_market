#pragma once
#include <variant>
#include <type_traits>
#include <tuple>
#include <vector>
#include <functional>
#include <boost/signals2/signal.hpp>


#include "Commands/BuyStockCommand.h"
#include "Commands/SellStockCommand.h"
#include "Commands/Command.h"
#include "Commands/UndoLatestCommand.h"
#include "Exceptions/BadCommandException.h"
#include "Queries/GetAllStockQuery.h"
#include "Queries/GetLatestStockQuery.h"

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

    template <typename QueryVar, typename CommandVar>
    class StockBroker
    {
        std::vector<CommandVar> all_commands;
        std::vector<boost::signals2::connection> connections;
        boost::signals2::signal<void(std::shared_ptr<QueryVar>)>& queries_sig_;
        boost::signals2::signal<void(std::shared_ptr<CommandVar>)>& command_sig_;

    public:
        StockBroker(boost::signals2::signal<void(std::shared_ptr<QueryVar>)>& queries_sig,
            boost::signals2::signal<void(std::shared_ptr<CommandVar>)>& command_sig)
            : queries_sig_(queries_sig),
            command_sig_(command_sig)
        {
            const std::function<void(std::shared_ptr<QueryVar>)> get_commands_from_stockbroker_f = [this](std::shared_ptr<QueryVar> query)
            {
                std::visit([this](auto&& q)
                    {
                        using T = std::decay_t<decltype(q)>;
                        if constexpr (std::is_same_v<T, GetAllStockQuery>)
                        {
                            auto buy_stocks = get_all_commands_of_type<stock::BuyStockCommand>();
                            auto sell_stocks = get_all_commands_of_type<stock::SellStockCommand>();
                            for (const auto& b : buy_stocks)
                            {
                                q.result.push_back(b);
                            }
                            for (const auto& s : sell_stocks)
                            {
                                q.result.push_back(s);
                            }
                        }
                    },
                    *query);
            };

            const std::function<void(std::shared_ptr<CommandVar>)> commands_f = [this](std::shared_ptr<CommandVar> variant)
            {
                std::visit([this](auto&& command)
                    {
                        handle_command(command);
                    }, *variant);
            };


            auto query_connection = queries_sig_.connect(get_commands_from_stockbroker_f);
            auto command_connection = command_sig_.connect(commands_f);
            connections.push_back(query_connection);
            connections.push_back(command_connection);
        }

        ~StockBroker()
        {
            for (int i = connections.size() - 1; i >= 0; --i)
            {
                connections[i].disconnect();
                connections.pop_back();
            }
        }

        template<typename Command>
        void handle_command(Command& command)
        {
            using T = std::decay_t<decltype(command)>;
            if constexpr (!hasExecute<T>)
            {
                throw stock::BadCommandException();
            }
            if constexpr (std::is_same_v<T, BuyStockCommand>)
            {
                handle(command);
            }
            else if constexpr (std::is_same_v<T, SellStockCommand>)
            {
                handle(command);
            }
            else if constexpr (std::is_same_v<T, UndoLatestCommand>)
            {
                handle(command);
            }
        }

        void handle(BuyStockCommand& buy_command)
        {
            std::cout << "Stockbroker doing buy...\n";
            buy_command.execute();
            all_commands.push_back(buy_command);

        }

        void handle(SellStockCommand& sell_command)
        {
            std::cout << "Stockbroker doing sell...\n";
            sell_command.execute();
            all_commands.push_back(sell_command);
        }

        void handle(UndoLatestCommand& undo_command)
        {
            auto latest = all_commands.back();
            if constexpr (hasUndo<std::decay_t<decltype(latest)>>)
            {
                std::cout << "Stockbroker undoing latest...\n";
                latest.undo();
                all_commands.pop_back();
            }

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
