#pragma once
#include <type_traits>


#include "BuyStockCommand.h"
#include "ListAllStocksCommand.h"
#include "ListAllTransactionsCommand.h"
#include "SellStockCommand.h"
#include "TraderAccountDepositCommand.h"
#include "UndoLatestCommand.h"
#include "../TypeList.h"

namespace stock
{
    // Inspiration from 19.5 (p. 428) of C++ Templates: The Complete Guide
    template<typename T>
    struct HasExecuteHelper
    {
        template<typename C, typename = decltype(std::declval<C>().execute())>
        static std::true_type test(void*);

        // test() fallback:
        template<typename>
        static std::false_type test(...);

        using Type = decltype(test<T>(nullptr));
        static constexpr bool value = std::is_same_v<decltype(test<T>(nullptr)), char>;
    };

    template<typename T>
    struct HasExecuteT : HasExecuteHelper<T>::Type {
    };

    template<typename T>
    constexpr bool hasExecute = HasExecuteT<T>::value;

    template<typename T>
    struct HasUndoHelper
    {
        template<typename C, typename = decltype(std::declval<C>().undo())>
        static std::true_type test(void*);

        // test() fallback:
        template<typename>
        static std::false_type test(...);

        using Type = decltype(test<T>(nullptr));
        static constexpr bool value = std::is_same_v<decltype(test<T>(nullptr)), char>;
    };

    template<typename T>
    struct HasUndoT : HasUndoHelper<T>::Type {
    };

    template<typename T>
    constexpr bool hasUndo = HasUndoT<T>::value;


    using commands_t = TypeList<stock::BuyStockCommand, stock::SellStockCommand, stock::ListAllTransactionsCommand, ListAllStocksCommand, stock::UndoLatestCommand, TraderAccountDepositAction>;
    using commands_var_t = stock::typelist_variant_t<commands_t>;
    using commands_sig_t = boost::signals2::signal<void(std::shared_ptr<commands_var_t>)>;
}
