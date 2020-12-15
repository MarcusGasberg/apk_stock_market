#pragma once
#include <memory>
#include <vector>
#include "../Commands/CommandBase.h"

namespace stock
{
    struct GetAllTransactionsQuery
    {
        std::vector<std::shared_ptr<CommandBase>> result;
    };
}
