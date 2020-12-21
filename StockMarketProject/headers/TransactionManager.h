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
    template<typename TBase>
    class FilterOfBaseVisitor
    {
        std::vector<std::shared_ptr<TBase>>& vec_;
    public:
        explicit FilterOfBaseVisitor(std::vector<std::shared_ptr<TBase>>& vec)
            : vec_(vec)
        {
        }

        template <typename C, typename = typename std::enable_if<std::is_base_of<TBase, C>::value>::type>
        void operator ()(C val)
        {
            vec_.emplace_back(std::make_shared<C>(val));
        }

        void operator()(...) const {}
    };

    class TransactionManager
    {
        std::vector<commands_var_t> all_transactions_;
        std::vector<boost::signals2::connection> connections_;

    public:
        TransactionManager(queries_sig_t& queries_sig,
                           commands_sig_t& commands_sig)
        {
            connect_queries(queries_sig);
            connect_commands(commands_sig);
        }

        ~TransactionManager()
        {
            for (auto i = connections_.rbegin(); i != connections_.rend(); ++i)
            {
                if(i->connected())
                {
                    i->disconnect();
                }
                connections_.erase(std::next(i).base());
            }
        }

        TransactionManager(const TransactionManager& other) = delete;

        TransactionManager(TransactionManager&& other) noexcept
            : all_transactions_(std::move(other.all_transactions_)),
              connections_(std::move(other.connections_))
        {
        }

        TransactionManager& operator=(const TransactionManager& other) = delete;

        TransactionManager& operator=(TransactionManager&& other) noexcept
        {
            if (this == &other)
                return *this;
            all_transactions_ = std::move(other.all_transactions_);
            connections_ = std::move(other.connections_);
            return *this;
        }

    private:
        void connect_commands(commands_sig_t& commands_sig)
        {
            const std::function<void(std::shared_ptr<commands_var_t>)> handle_commands_f =
                [this](const std::shared_ptr<commands_var_t> variant)
            {
                std::visit([this](auto&& command)
                    {
                        handle_command(command);
                    }, *variant);
            };

            const auto command_connection = commands_sig.connect(handle_commands_f);
            connections_.push_back(command_connection);
        }

        template<typename Command>
        void handle_command(Command& command)
        {
            using T = std::decay_t<decltype(command)>;

            static_assert(hasExecute<T>);

            if constexpr (std::is_base_of_v<TransactionBase, T>)
            {
                static_assert(hasUndo<T>);
                auto success = command.execute();
                if (success)
                {
                    all_transactions_.emplace_back(command);
                }
            }
            else if constexpr (std::is_same_v<T, UndoLatestCommand>)
            {
                undo_latest_transaction(command);
            }
        }

        void connect_queries(queries_sig_t& queries_sig)
        {
            const std::function<void(std::shared_ptr<queries_var_t>)> get_transactions_f =
                [this](const std::shared_ptr<queries_var_t> query)
            {
                std::visit([this](auto&& q)
                    {
                        using T = std::decay_t<decltype(q)>;
                        if constexpr (std::is_same_v<T, GetAllTransactionsQuery>)
                        {
                            auto transactions = get_all_transactions();
                            q.append_result(std::move(transactions));
                        }
                    },
                    *query);
            };
            const auto query_connection = queries_sig.connect(get_transactions_f);
            connections_.push_back(query_connection);
        }

        void undo_latest_transaction(UndoLatestCommand& undo_command)
        {
            std::function<void()> undo_latest_f = [this]()
            {
                if (all_transactions_.empty())
                {
                    std::cout << "No more transactions in stock broker...\n";
                    return;
                }
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
            FilterOfBaseVisitor<TransactionBase> visitor(commands);
            for (auto& transaction : all_transactions_)
            {
                std::visit(visitor, transaction);
            }
            return commands;
        }
    };
} // namespace stock
