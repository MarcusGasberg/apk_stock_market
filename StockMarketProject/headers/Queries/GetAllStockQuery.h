#pragma once
#include <memory>
#include <vector>

#include "../Models/Stock.h"

namespace stock
{
    struct GetAllStockQuery
    {
        std::vector<Stock> result;
    };
}
