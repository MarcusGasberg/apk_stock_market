#pragma once
#include "GetAllStockQuery.h"
#include "GetAllTransactionsQuery.h"
#include "GetLatestStockQuery.h"
#include "GetStockPriceQuery.h"
#include "GetStockQuery.h"
#include "../TransactionManager.h"
#include "../TypeList.h"

namespace stock
{
    using queries_t = TypeList<GetAllTransactionsQuery, GetLatestStockQuery,
                               GetStockQuery, GetAllStockQuery, GetStockPriceQuery>;
    using queries_var_t = typelist_variant_t<queries_t>;
    using queries_sig_t = boost::signals2::signal<void(std::shared_ptr<queries_var_t>)>;
}
