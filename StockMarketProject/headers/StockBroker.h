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
#include "Commands/ListAllStockCommand.h"
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
        boost::signals2::signal<void(QueryVar&)>& queries_sig_;
        boost::signals2::signal<void(CommandVar&)>& command_sig_;

    public:
        StockBroker(boost::signals2::signal<void(QueryVar&)>& queries_sig,
            boost::signals2::signal<void(CommandVar&)>& command_sig)
            : queries_sig_(queries_sig),
              command_sig_(command_sig)
        {
            const std::function<void(QueryVar&)> get_commands_from_stockbroker_f = [this](QueryVar& query)
            {
                std::visit([this](auto&& q)
                    {
                        using T = std::decay_t<decltype(q)>;
                        if constexpr (std::is_same_v<T, stock::GetAllStockQuery>)
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
                    }
                , query);
            };

            const std::function<void(QueryVar&)> get_latest_stock_f = [this](QueryVar& query)
            {
                std::visit([this](auto&& q)
                    {
                        using T = std::decay_t<decltype(q)>;
                        if constexpr (std::is_same_v<T, stock::GetLatestStockQuery>)
                        {
                        }
                    }
                , query);
            };

            const std::function<void(CommandVar&)> commands_f = [this](CommandVar& variant)
            {
                std::visit([&](auto&& command)
                    {
                        using T = std::decay_t<decltype(command)>;
                        if constexpr (!stock::hasExecute<T>)
                        {
                            return;
                        }
                        handle_command(command);
                    }, variant);
            };


            queries_sig_.connect(get_commands_from_stockbroker_f);
            command_sig_.connect(commands_f);
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
                buy(command);
            }
            else if constexpr (std::is_same_v<T, SellStockCommand>)
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
