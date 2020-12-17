#pragma once
#include <memory>
#include <string>

#include "../Models/Stock.h"


namespace stock
{
    class GetStockQuery
    {
        std::string stock_id_;
    public:
        GetStockQuery() = default;

        explicit GetStockQuery(std::string&& stock_id)
            : stock_id_(std::move(stock_id))
        {
        }

        [[nodiscard]] std::string get_stock_id() const
        {
            return stock_id_;
        }

        std::shared_ptr<Stock> result;
    };
}
