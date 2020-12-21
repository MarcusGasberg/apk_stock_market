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

        template <typename C, typename = typename std::enable_if<std::is_base_of<T, C>::value>::type>

        void operator ()(C val)
        {
            vec_.push_back(std::make_shared<C>(val));
        }

        void operator()(...) const {}
    };

    class TransactionManager
    {
        std::vector<commands_var_t> all_transactions_;
        std::vector<boost::signals2::connection> connections_;

    public:
        TransactionManager(queries_sig_t& queries_sig,
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
            connections_.push_back(query_connection);
            connections_.push_back(command_connection);
        }

        ~TransactionManager()
        {
            for (int i = connections_.size() - 1; i >= 0; --i)
            {
                if (connections_[i].connected())
                {
                    connections_[i].disconnect();
                    connections_.pop_back();
                }
            }
        }

        template<typename Command>
        void handle_command(Command& command)
        {
            using T = std::decay_t<decltype(command)>;

            static_assert(hasExecute<T>);

            if constexpr (std::is_base_of_v<TransactionBase, T>)
            {
                static_assert(hasUndo<T>);
                do_transaction(command);
            }
            else if constexpr (std::is_same_v<T, UndoLatestCommand>)
            {
                undo_latest_transaction(command);
            }
        }

        template<typename Command>
        void do_transaction(Command transaction)
        {
            auto success = transaction.execute();
            if(success)
            {
                all_transactions_.emplace_back(transaction);
            }
        }

        void undo_latest_transaction(UndoLatestCommand& undo_command)
        {
            if (all_transactions_.empty())
            {
                std::cout << "No more transactions in stock broker...\n";
                return;
            }

            std::function<void()> undo_latest_f = [this]()
            {
                auto latest = all_transactions_.back();

                std::visit([&](auto&& val)
                    {
                        if constexpr (hasUndo<std::decay_t<decltype(val)>>)
                        {
                            std::cout << "Stockbroker undoing latest...\n";
                            val.undo();
                            all_transactions_.pop_back();
                        }
                    }, latest);
            };

            undo_command.add_undo_action(undo_latest_f);
        }

        std::vector<std::shared_ptr<TransactionBase>> get_all_transactions()
        {
            std::vector<std::shared_ptr<TransactionBase>> commands;
            to_vector_visitor<TransactionBase> visitor(commands);
            for (auto& transaction : all_transactions_)
            {
                std::visit(visitor, transaction);
            }
            return commands;
        }
    };
} // namespace stock
