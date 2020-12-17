//
// Created by stud on 11/30/20.
//

#ifndef STOCKMARKETPROJECT_TRADERACCOUNT_H
#define STOCKMARKETPROJECT_TRADERACCOUNT_H
#include <utility>
#include <vector>

#include "Policies/StandardTraderPolicy.h"
#include "../StockMediator/Mediator.h"
#include "../Models/Stock.h"
#include "TraderTopics.h"

namespace stock {
    template<typename TraderPolicy>
    class TraderAccount;

    // Take some commission for trade based on behaviour policy (Student, Plus-Member, Standard)
    // Withdraw appropriated funds from account manager (either by reference or signal).
    template<typename TraderPolicy = StandardTraderPolicy>
    class TraderAccount {
    private:
        std::string id_;
        int balance{};
        std::vector<Stock> ownedStocks_;
        // TODO: Make shared ptr - several trader account can have same mediator?
        Mediator<void, Stock&>& mediator_;
    public:
        explicit TraderAccount(std::string&& id, Mediator<void, Stock&>& mediator)
                : id_(std::move(id)), mediator_(mediator) {}

       std::string get_id() const {
            return id_;
       }

        void deposit(int amount)
        {
            balance += amount;
        }

        std::shared_ptr<Stock> get_stock(std::string stock_id)
        {
            std::shared_ptr<Stock> result;
            auto stock_itr = std::find_if(ownedStocks_.begin(), ownedStocks_.end(), [&stock_id](Stock& st)
                {
                    return stock_id == st.getStockId();
                });

            if (stock_itr != ownedStocks_.end())
            {
                result = std::make_shared<Stock>(*stock_itr);
            }

            return result;
        }

        bool buy_stock(Stock & stock, int price) {
            auto commission = TraderPolicy::calculate_commission(stock.getAmount(), stock.getPrice().price_);
            balance -= stock.getAmount() * stock.getPrice().price_ + commission;

            ownedStocks_.push_back(stock);

            std::cout << "Bought " << stock.getStockId() << ", new balance is: " << balance << "\n";
            mediator_.notify(TOPICS[TraderTopics::BUY], stock);
            return true;
        }

        bool sell_stock(Stock& stock, int price) {
            auto stock_itr = std::remove_if(ownedStocks_.begin(), ownedStocks_.end(), [&stock](Stock& st)
                {
                    return stock.getStockId() == st.getStockId();
                });

            if(stock_itr == ownedStocks_.end())
            {
                std::cout << "You don't own the stock: " << stock.getStockId() << "\n";
                return false;
            }

            auto commission = TraderPolicy::calculate_commission(stock_itr->getAmount(), stock_itr->getPrice().price_);
            balance += stock_itr->getAmount() * stock_itr->getPrice().price_ - commission;
            std::cout << "Sold " << stock_itr->getStockId() << ", new balance is: " << balance << "\n";

            mediator_.notify(TOPICS[TraderTopics::SELL], *stock_itr);
            return true;
        }
    };
}

#endif //STOCKMARKETPROJECT_TRADERACCOUNT_H
