#pragma once
#include <vector>
#include <memory>
#include "../Commands/TransactionBase.h"

namespace stock
{
    struct GetAllTransactionsQuery
    {
        std::vector<std::shared_ptr<TransactionBase>> result;
    };
}
