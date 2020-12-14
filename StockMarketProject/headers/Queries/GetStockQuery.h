#pragma once
#include <memory>
#include <string>

#include "../StockMediator/Stock.h"


namespace stock
{
    class GetStockQuery
    {
        std::string stock_id_;
    public:
        explicit GetStockQuery(std::string stock_id)
            : stock_id_(stock_id)
        {
        }

        [[nodiscard]] std::string get_account_id() const
        {
            return stock_id_;
        }

        std::shared_ptr<Stock> result;
    };
}
