#pragma once
#include <vector>
#include "../Commands/CommandBase.h"

namespace stock
{
    struct GetAllStockQuery
    {
        std::vector<std::shared_ptr<CommandBase>> result;
    };
}
