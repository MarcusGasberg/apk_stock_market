//
// Created by stud on 12/15/20.
//

#ifndef STOCKMARKETPROJECT_STOCKBROKER_H
#define STOCKMARKETPROJECT_STOCKBROKER_H

#include <string>
#include "../Models/Stock.h"
#include "../Queries/GetAllStockQuery.h"
#include "../Queries/Queries.h"
#include "../StockPrices/PriceProvider.h"
#include "../Exceptions/NoPriceException.h"

namespace stock {
    class StockBroker {
        std::string name_;
        std::shared_ptr<PriceProvider> price_provider_;
        std::vector<Stock> stocks_for_sale;
        const int delay_ = 1;
        std::vector<boost::signals2::connection> connections_;
        std::shared_ptr<StockMediator<void, Stock&>> mediator_;
    public:
        StockBroker(std::string&& name, std::shared_ptr<PriceProvider> price_provider, queries_sig_t& queries_sig, std::shared_ptr<StockMediator < void, Stock&>> mediator) :
            name_(std::move(name)),
            price_provider_(price_provider), mediator_(mediator)
        {
            const std::function<void(std::shared_ptr<queries_var_t>)> get_stock_f = [this](const std::shared_ptr<queries_var_t> query_var)
            {
                std::visit([this](auto&& query)
                    {
                        using T = std::decay_t<decltype(query)>;
                        if constexpr (std::is_same_v<T, GetStockQuery> || std::is_same_v<T, GetAllStockQuery> || std::is_same_v<T, GetStockPriceQuery>)
                        {
                            handle(query);
                        }
                    }, 
                    *query_var);
            };

            queries_sig.connect(get_stock_f);

            auto buy_connection = mediator_->subscribe(TOPICS[TraderTopics::BUY], &StockBroker::remove_bought_stock, this);
            auto sell_connection = mediator_->subscribe(TOPICS[TraderTopics::SELL], &StockBroker::add_stock, this);
            connections_.push_back(buy_connection);
            connections_.push_back(sell_connection);

        }

        virtual ~StockBroker() {
            for (int i = connections_.size() - 1; i >= 0; --i)
            {
                if (connections_[i].connected())
                {
                    connections_[i].disconnect();
                    connections_.pop_back();
                }
            }
        }

        void remove_bought_stock(Stock& stock)
        {
            const auto remove_itr = std::remove_if(stocks_for_sale.begin(), stocks_for_sale.end(), [&stock](Stock& st)
            {
                return st.getStockId() == stock.getStockId();
            });
            if(remove_itr != stocks_for_sale.end())
            {
                stocks_for_sale.erase(remove_itr);
            }
        }

        void add_stock(Stock& stock) {
            if(stock.getStockProviderId() == this->getName())
                stocks_for_sale.push_back(stock);
        }

        void add_stock_for_sale(const Stock& stock)
        {
            if(stock.getStockProviderId() == this->getName())
                stocks_for_sale.push_back(stock);
        }

        void stockHasBeenBought(Stock&& stock) {
            std::cout << "Stock " << stock.getStockId() << " was bought";
        }

        const std::string& getName() const {
            return name_;
        }

        void setName(const std::string& name) {
            StockBroker::name_ = name;
        }

    private:
        void handle(GetStockQuery& query)
        {
            auto stock_id = std::make_shared<std::string>(query.get_stock_id());

            query.result = std::async(std::launch::async, [stock_id, this]()
                {
                    std::cout << "Finding stock " << *stock_id << "...\n";
                    std::this_thread::sleep_for(std::chrono::seconds(delay_));

                    std::shared_ptr<Stock> result;
                    auto stock = std::find_if(stocks_for_sale.begin(), stocks_for_sale.end(), [stock_id, this](Stock st)
                        {
                            return st.getStockId() == *stock_id;
                        });

                    if (stock == stocks_for_sale.end())
                        return result;

                    auto price = price_provider_->get_price(stock->getStockId());

                    if(!price) {
                        throw NoPriceException(stock->getStockId());
                    }

                    stock->setPrice(price);
                    result = std::make_shared<Stock>(*stock);

                    return result;
                });
        }

        void handle(GetAllStockQuery& query)
        {
            query.result = std::async(std::launch::async, [this]()
                {
                    auto stocks = stocks_for_sale;
                    for (auto&& stock : stocks)
                    {
                        auto price = price_provider_->get_price(stock.getStockId());
                        if (!price) {
                            throw NoPriceException(stock.getStockId());
                        }

                        stock.setPrice(price);
                    }
                    return stocks;
                });
        }

        void handle(GetStockPriceQuery& query)
        {
            auto stock_id = std::make_shared<std::string>(query.get_stock_id());
            query.result = std::async(std::launch::async, [stock_id, this]()
                {
                    std::cout << "Getting price of " << *stock_id << "...\n";
                    std::this_thread::sleep_for(std::chrono::seconds(delay_));

                    auto price = price_provider_->get_price(std::move(*stock_id));
                    if (!price) {
                        throw NoPriceException(*stock_id);
                    }
                    return price;
                });
        }
    };
}
#endif //STOCKMARKETPROJECT_STOCKBROKER_H
