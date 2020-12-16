#pragma once
#include "GetLatestStockQuery.h"
#include "GetStockQuery.h"
#include "GetTraderAccountQuery.h"
#include "../StockBroker.h"

namespace stock
{
    using queries_t = TypeList<GetAllTransactionsQuery, GetLatestStockQuery, GetTraderAccountQuery,
                               GetStockQuery>;
    using queries_var_t = typelist_variant_t<queries_t>;
}
