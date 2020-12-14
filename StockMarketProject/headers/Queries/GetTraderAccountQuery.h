#pragma once
#include <memory>
#include <string>

#include "../TraderAccount/TraderAccount.h"

namespace stock
{
    class GetTraderAccountQuery
    {
        std::string account_id_;
    public:
        explicit GetTraderAccountQuery(std::string account_id)
            : account_id_(account_id)
        {
        }

        [[nodiscard]] std::string get_account_id() const
        {
            return account_id_;
        }

        std::shared_ptr<TraderAccount<>> result;
    };
}
