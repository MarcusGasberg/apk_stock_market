//
// Created by stud on 11/30/20.
//

#ifndef STOCKMARKETPROJECT_TRADERACCOUNT_H
#define STOCKMARKETPROJECT_TRADERACCOUNT_H
#include <utility>
#include <vector>

#include "Policies/StandardTraderPolicy.h"
#include "../StockMediator/StockMediator.h"
#include "../Models/Stock.h"
#include "TraderTopics.h"
#include "../Exceptions/NoPriceException.h"
#include "../Exceptions/NoStockException.h"
#include "../Queries/GetStockQuery.h"
#include "../Queries/Queries.h"

namespace stock {
    template<typename TraderPolicy>
    class TraderAccount;

    // Take some commission for trade based on behaviour policy (Student, Plus-Member, Standard)
    // Withdraw appropriated funds from account manager (either by reference or signal).
    template<typename TraderPolicy = StandardTraderPolicy>
    class TraderAccount {
    private:
        std::string id_;
        int balance_{};
        std::vector<Stock> owned_stocks_;
        std::shared_ptr<StockMediator<void, Stock&>> mediator_;
        queries_sig_t& query_sig_;
    public:
        explicit TraderAccount(std::string&& id, std::shared_ptr<StockMediator<void, Stock&>> mediator, queries_sig_t& query_sig)
                : id_(std::move(id)), mediator_(mediator), query_sig_(query_sig) {}

       std::string get_id() const {
            return id_;
       }

        void deposit(int amount)
        {
            balance_ += amount;
        }

        int get_balance() const
        {
            return balance_;
        }

        std::shared_ptr<Stock> get_stock(std::string stock_id)
        {
            std::shared_ptr<Stock> result;
            auto stock_itr = std::find_if(owned_stocks_.begin(), owned_stocks_.end(), [&stock_id](Stock& st)
                {
                    return stock_id == st.getStockId();
                });

            if (stock_itr != owned_stocks_.end())
            {
                result = std::make_shared<Stock>(*stock_itr);
            }

            return result;
        }

        std::vector<Stock> owned_stocks() const
        {
            return owned_stocks_;
        }

        bool buy_stock(const std::string& stock_id) {
            auto stock_query = launch_stock_query(stock_id);

            auto price_query = launch_price_query(stock_id);

            const auto timeout = std::chrono::milliseconds(100);

            std::shared_ptr<Stock> stock;
            std::shared_ptr<Price> price;

            try
            {
                while (stock_query.valid() || price_query.valid())
                {
                    if (stock_query.valid() && stock_query.wait_for(timeout) == std::future_status::ready) {
                        stock = stock_query.get();
                        std::cout << "Stock query is done: " << *stock << "\n";
                    }
                    if (price_query.valid() && price_query.wait_for(timeout) == std::future_status::ready) {
                        price = price_query.get();
                        std::cout << "Price query is done: " << price->price_ << "\n";
                    }
                }
            }
            catch (NoPriceException& exception)
            {
                std::cout << exception.what();
                return false;
            }

            stock->setPrice(price);

            auto commission = TraderPolicy::calculate_commission(stock->getAmount(), stock->getPrice()->price_);
            auto subtract_amount = stock->getAmount() * stock->getPrice()->price_ + commission;
            if (balance_ - subtract_amount < 0)
            {
                std::cout << "Insufficient funds: " << balance_ << ". Needed: " << subtract_amount << "\n";
                return false;
            }

            balance_ -= subtract_amount;
            owned_stocks_.push_back(*stock);

            std::cout << "Bought " << stock->getStockId() << ", new balance is: " << balance_ << "\n";
            mediator_->notify(TOPICS[TraderTopics::BUY], *stock);
            return true;
        }

        bool sell_stock(const std::string& stock_id) {
            auto stock_itr = std::remove_if(owned_stocks_.begin(), owned_stocks_.end(), [&stock_id](Stock& st)
                {
                    return stock_id == st.getStockId();
                });

            if (stock_itr == owned_stocks_.end())
            {
                std::cout << "You don't own the stock: " << stock_id << "\n";
                return false;
            }

            auto price_query = launch_price_query(stock_id);

            const auto timeout = std::chrono::milliseconds(100);

            std::shared_ptr<Price> price;
            try
            {
                while (price_query.valid())
                {
                    if (price_query.valid() && price_query.wait_for(timeout) == std::future_status::ready) {
                        price = price_query.get();
                        std::cout << "Price query is done: " << price->price_ << "\n";
                    }
                }
            }
            catch (NoPriceException& exception)
            {
                std::cout << exception.what();
                return false;
            }

            stock_itr->setPrice(price);

            auto commission = TraderPolicy::calculate_commission(stock_itr->getAmount(), stock_itr->getPrice()->price_);
            balance_ += stock_itr->getAmount() * stock_itr->getPrice()->price_ - commission;
            std::cout << "Sold " << stock_itr->getStockId() << ", new balance is: " << balance_ << "\n";

            mediator_->notify(TOPICS[TraderTopics::SELL], *stock_itr);

            owned_stocks_.erase(stock_itr);

            return true;
        }

        std::future<std::shared_ptr<Stock>> launch_stock_query(const std::string& stock_id)
        {
            return std::async(std::launch::async, [&stock_id, this]()
                {
                    const std::shared_ptr<queries_var_t> queries_var = std::make_shared<queries_var_t>(GetStockQuery(std::string{ stock_id }));
                    query_sig_(queries_var);

                    const GetStockQuery queries_result = std::move(std::get<GetStockQuery>(*queries_var));
                    auto stock = queries_result.result;

                    std::this_thread::sleep_for(std::chrono::seconds(2));

                    if (!stock)
                    {
                        std::cout << "Stock not found for id: " << stock_id << "\n";
                    }

                    return stock;
                });
        }

        std::future<std::shared_ptr<Price>> launch_price_query(const std::string& stock_id)
        {
            return std::async(std::launch::async, [&stock_id, this]()
                {
                    const std::shared_ptr<queries_var_t> queries_var = std::make_shared<queries_var_t>(GetStockPriceQuery(std::string{ stock_id }));
                    query_sig_(queries_var);
                    const GetStockPriceQuery price_result = std::move(std::get<GetStockPriceQuery>(*queries_var));

                    const auto price = price_result.result;

                    std::this_thread::sleep_for(std::chrono::seconds(3));

                    if (!price)
                    {
                        std::cout << "Price not found for stock id: " << stock_id << "\n";
                    }

                    return price;
                });
        }
    };
}

#endif //STOCKMARKETPROJECT_TRADERACCOUNT_H
