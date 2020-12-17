//
// Created by stud on 12/15/20.
//

#ifndef STOCKMARKETPROJECT_STOCKPROVIDER_H
#define STOCKMARKETPROJECT_STOCKPROVIDER_H

#include <string>
#include "../Models/Stock.h"
#include "../Queries/GetAllStockQuery.h"
#include "../Queries/Queries.h"
#include "../StockPrices/PriceProvider.h"

namespace stock {
    class StockProvider {
    private:
        std::string name_;
        PriceProvider& price_provider_;
        std::vector<Stock> stocks_for_sale;
        Mediator<void, Stock&>& mediator_;
    public:
        StockProvider(std::string&& name, PriceProvider& price_provider, queries_sig_t& queries_sig, Mediator<void, Stock&>& mediator) :
            name_(std::move(name)),
            price_provider_(price_provider), mediator_(mediator)
        {
            const std::function<void(std::shared_ptr<queries_var_t>)> get_stock_f = [this](const std::shared_ptr<queries_var_t> query_var)
            {
                std::visit([this](auto&& query)
                    {
                        using T = std::decay_t<decltype(query)>;
                        if constexpr (std::is_same_v<T, GetStockQuery>)
                        {
                            auto stock = std::find_if(stocks_for_sale.begin(), stocks_for_sale.end(), [&query](Stock st)
                                {
                                    return st.getStockId() == query.get_stock_id();
                                });

                            if (stock == stocks_for_sale.end())
                                return;

                            stock->setPrice(std::make_shared<Price>(price_provider_.get_price(stock->getStockId())));
                            query.result = std::make_shared<Stock>(*stock);
                        }
                        if constexpr (std::is_same_v<T, GetAllStockQuery>)
                        {
                            std::for_each(stocks_for_sale.begin(), stocks_for_sale.end(), [&](auto stock){
                                stock.setPrice(std::make_shared<Price>(price_provider_.get_price(stock.getStockId())));
                                query.result.push_back(stock);
                            });
                        }
                        if constexpr (std::is_same_v<T, GetStockPriceQuery>)
                        {
                            query.result = std::make_shared<Price>(price_provider_.get_price(query.get_stock_id()));
                        }
                    }, 
                    *query_var);
            };

            queries_sig.connect(get_stock_f);

            mediator_.subscribe(TOPICS[TraderTopics::BUY], &StockProvider::remove_bought_stock, this);
            mediator_.subscribe(TOPICS[TraderTopics::SELL], &StockProvider::add_sold_stock, this);
        }

        void remove_bought_stock(Stock& stock)
        {
            const auto remove_itr = std::remove_if(stocks_for_sale.begin(), stocks_for_sale.end(), [&stock](Stock& st)
            {
                return st.getStockId() == stock.getStockId();
            });
            stocks_for_sale.erase(remove_itr);
        }

        void add_sold_stock(Stock& stock)
        {
            stocks_for_sale.push_back(stock);
        }

        void add_stock_for_sale(const Stock& stock)
        {
            stocks_for_sale.push_back(stock);
        }

        void stockHasBeenBought(Stock&& stock) {
            std::cout << "Stock " << stock.getStockId() << " was bought";
        }

        const std::string& getName() const {
            return name_;
        }

        void setName(const std::string& name) {
            StockProvider::name_ = name;
        }

    };
}
#endif //STOCKMARKETPROJECT_STOCKPROVIDER_H
