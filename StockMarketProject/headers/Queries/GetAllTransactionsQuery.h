#pragma once
#include <vector>
#include <memory>
#include "../Commands/TransactionBase.h"

namespace stock
{
    class GetAllTransactionsQuery
    {
        std::vector<std::shared_ptr<TransactionBase>> result_;
    public:
        void append_result(std::vector<std::shared_ptr<TransactionBase>>&& sub_result)
        {
            result_.insert(result_.end(), sub_result.begin(), sub_result.end());
        }

        [[nodiscard]] std::vector<std::shared_ptr<TransactionBase>> get_result() const
        {
            return result_;
        }
    };
}
