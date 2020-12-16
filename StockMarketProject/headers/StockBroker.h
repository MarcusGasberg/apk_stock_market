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
#include "Queries/GetAllTransactionsQuery.h"
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

        template <typename C>
        void operator ()(C val)
        {
            if constexpr (std::is_base_of_v<T, C>)
            {
                vec_.push_back(std::make_shared<C>(val));
            }
        }

        template<typename >
        void operator()(...) const {}
    };

    template <typename QueryVar, typename CommandVar>
    class StockBroker
    {
        std::vector<CommandVar> all_transactions;
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
                        if constexpr (std::is_same_v<T, GetAllTransactionsQuery>)
                        {
                            auto transactions = get_all_transactions();
                            for (auto transaction : transactions)
                            {
                                q.result.push_back(std::move(transaction));
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
                throw stock::BadCommandException("No execute");
            }
            if constexpr (std::is_base_of_v<TransactionBase, T> && hasUndo<std::decay_t<T>>)
            {
                do_transaction(command);
            }
            else if constexpr (std::is_same_v<T, UndoLatestCommand>)
            {
                undo_latest_transaction(command);
            }
        }

        template<typename TTransaction>
        void do_transaction(TTransaction transaction)
        {
            transaction.execute();
            all_transactions.push_back(transaction);
        }

        void undo_latest_transaction(UndoLatestCommand& undo_command)
        {
            if(all_transactions.empty())
            {
                std::cout << "No more transactions...\n";
                return;
            }

            auto latest =  all_transactions.back();

            std::visit([&](auto&& val)
                {
                    if constexpr (hasUndo<std::decay_t<decltype(val)>> )
                    {
                        std::cout << "Stockbroker undoing latest...\n";
                        val.undo();
                        all_transactions.pop_back();
                    }
                }, latest);
        }

        std::vector<std::shared_ptr<TransactionBase>> get_all_transactions()
        {
            std::vector<std::shared_ptr<TransactionBase>> commands;
            to_vector_visitor<TransactionBase> visitor(commands);
            for (size_t i = 0; i < all_transactions.size(); ++i)
            {
                std::visit(visitor, all_transactions[i]);
            }
            return commands;
        }

        template <typename Command>
        void do_execute(Command& command)
        {
            command.execute();
            all_transactions.push_back(command);
        }
    };
} // namespace stock
