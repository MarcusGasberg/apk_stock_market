#pragma once
#include <vector>
#include <memory>
#include "../Commands/CommandBase.h"

namespace stock
{
    struct GetAllTransactionsQuery
    {
        std::vector<std::shared_ptr<CommandBase>> result;
    };
}
