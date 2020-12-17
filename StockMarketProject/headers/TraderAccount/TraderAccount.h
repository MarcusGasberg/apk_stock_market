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
        std::vector<Stock> ownedStocks_;
        // TODO: Make shared ptr - several trader account can have same mediator?
        Mediator<void, Stock>& mediator_;
    public:
        explicit TraderAccount(std::string&& id, Mediator<void, Stock>& mediator)
                : id_(std::move(id)), mediator_(mediator) {}

       std::string get_id() const {
            return id_;
        }

        // TODO: Perfect forwarding?
        bool BuyStock(Stock & stock, int price) {
            // TODO: Logic with policy and account manager
            std::cout << "Buying " << stock.getStockId() << std::endl;
            mediator_.notify(TOPICS[TraderTopics::BUY], std::move(stock));
            return true;
        }

        bool SellStock(Stock & stock, int price) {
            // TODO: Logic with policy and account manager
            std::cout << "Selling " << stock.getStockId() << std::endl;
            mediator_.notify(TOPICS[TraderTopics::BUY], std::move(stock));
            return true;
        }
    };
}

#endif //STOCKMARKETPROJECT_TRADERACCOUNT_H
