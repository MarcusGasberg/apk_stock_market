#pragma once
#include <variant>
#include <type_traits>
#include <vector>
#include <functional>
#include <boost/signals2/signal.hpp>


#include "Commands/BuyStockCommand.h"
#include "Commands/SellStockCommand.h"
#include "Commands/Commands.h"
#include "Commands/UndoLatestCommand.h"
#include "Exceptions/BadCommandException.h"
#include "Queries/GetAllTransactionsQuery.h"
#include "Queries/Queries.h"

namespace stock
{
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

    class StockBroker
    {
        std::vector<commands_var_t> all_transactions;
        std::vector<boost::signals2::connection> connections;

    public:
        StockBroker(queries_sig_t& queries_sig,
            commands_sig_t& command_sig)
        {
            const std::function<void(std::shared_ptr<queries_var_t>)> get_commands_from_stockbroker_f =
                [this](const std::shared_ptr<queries_var_t> query)
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

            const std::function<void(std::shared_ptr<commands_var_t>)> commands_f = 
                [this](const std::shared_ptr<commands_var_t> variant)
            {
                std::visit([this](auto&& command)
                    {
                        handle_command(command);
                    }, *variant);
            };

            const auto query_connection = queries_sig.connect(get_commands_from_stockbroker_f);
            const auto command_connection = command_sig.connect(commands_f);
            connections.push_back(query_connection);
            connections.push_back(command_connection);
        }

        ~StockBroker()
        {
            for (int i = connections.size() - 1; i >= 0; --i)
            {
                if (connections[i].connected())
                {
                    connections[i].disconnect();
                    connections.pop_back();
                }
            }
        }

        template<typename Command>
        void handle_command(Command& command)
        {
            using T = std::decay_t<decltype(command)>;
            if constexpr (!hasExecute<T>)
            {
                throw BadCommandException("No execute");
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


        void do_transaction(BuyStockCommand transaction)
        {
            transaction.execute();
            all_transactions.emplace_back(transaction);
        }

        void do_transaction(SellStockCommand transaction)
        {

            transaction.execute();
            all_transactions.emplace_back(transaction);
        }

        void undo_latest_transaction(UndoLatestCommand& undo_command)
        {
            if (all_transactions.empty())
            {
                std::cout << "No more transactions in stock broker...\n";
                return;
            }

            std::function<void()> undo_latest_f = [this]()
            {
                auto latest = all_transactions.back();

                std::visit([&](auto&& val)
                    {
                        if constexpr (hasUndo<std::decay_t<decltype(val)>>)
                        {
                            std::cout << "Stockbroker undoing latest...\n";
                            val.undo();
                            all_transactions.pop_back();
                        }
                    }, latest);
            };

            undo_command.add_undo_action(undo_latest_f);
        }

        std::vector<std::shared_ptr<TransactionBase>> get_all_transactions()
        {
            std::vector<std::shared_ptr<TransactionBase>> commands;
            to_vector_visitor<TransactionBase> visitor(commands);
            for (auto& all_transaction : all_transactions)
            {
                std::visit(visitor, all_transaction);
            }
            return commands;
        }
    };
} // namespace stock
