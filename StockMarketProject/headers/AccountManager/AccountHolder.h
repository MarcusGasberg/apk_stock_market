
#include <vector>
#include <boost/signals2/signal.hpp>


#include "../Queries/GetTraderAccountQuery.h"
#include "../TraderAccount/TraderAccount.h"

namespace stock
{
    template <typename QueryVar>
    class AccountHolder
    {
        boost::signals2::connection connection_;
        std::vector<std::shared_ptr<TraderAccount<>>> trader_accounts_;
        boost::signals2::signal<void(std::shared_ptr<QueryVar>)>& queries_sig_;
    public:
        explicit AccountHolder(boost::signals2::signal<void(std::shared_ptr<QueryVar>)>& queries_sig)
            : queries_sig_(queries_sig)
        {
            const std::function<void(std::shared_ptr<QueryVar>)> get_account_f = [this](std::shared_ptr<QueryVar> variant)
            {
                std::visit([this](auto&& query)
                {
                    using T = std::decay_t<decltype(query)>;
                    if constexpr (std::is_same_v<T, GetTraderAccountQuery>)
                    {
                        auto result = std::find_if(trader_accounts_.begin(), trader_accounts_.end(), [&query](const std::shared_ptr<TraderAccount<>> x) { return x->get_id() == query.get_account_id(); });

                        query.result = std::move(*result);
                    }
                }, *variant);
            };

            connection_ = queries_sig_.connect(get_account_f);
        }

        ~AccountHolder() { connection_.disconnect(); }

        void add_account(std::shared_ptr<TraderAccount<>> account)
        {
            trader_accounts_.push_back(account);
        }
    };
}
