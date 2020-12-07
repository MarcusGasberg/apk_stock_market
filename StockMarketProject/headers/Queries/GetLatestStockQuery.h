#pragma once

#include <memory>

#include "../Commands/CommandBase.h"

namespace stock
{
    struct GetLatestStockQuery
    {
        std::shared_ptr<CommandBase> result;
    };
}
