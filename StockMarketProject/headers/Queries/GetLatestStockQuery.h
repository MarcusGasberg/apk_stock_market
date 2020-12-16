#pragma once

#include <memory>

#include "../Commands/TransactionBase.h"

namespace stock
{
    struct GetLatestStockQuery
    {
        std::shared_ptr<TransactionBase> result;
    };
}
