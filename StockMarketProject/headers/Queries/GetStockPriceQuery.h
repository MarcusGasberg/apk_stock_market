#pragma once

#include <future>

#include "../Models/Stock.h"

namespace stock
{
    class GetStockPriceQuery
    {
        std::string stock_id_;
    public:
        explicit GetStockPriceQuery(std::string&& stock_id)
            : stock_id_(std::move(stock_id))
        {
        }

        std::string get_stock_id() const
        {
            return stock_id_;
        }

        std::future<std::shared_ptr<Price>> result;
    };
}
