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
    public:
        StockProvider(std::string&& name, PriceProvider& price_provider, queries_sig_t& queries_sig) :
            name_(std::move(name)),
            price_provider_(price_provider)
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
                    }, 
                    *query_var);
            };

            queries_sig.connect(get_stock_f);
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
