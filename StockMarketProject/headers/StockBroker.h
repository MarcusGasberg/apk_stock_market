#pragma once
#include <variant>
#include <type_traits>
#include <tuple>
#include <vector>


#include "Commands/BuyStockCommand.h"
#include "Commands/ListAllStockCommand.h"
#include "Commands/SellStockCommand.h"
#include "Exceptions/BadCommandException.h"

namespace stock
{
    template <typename... T>
    struct TypeList
    {
        template<template<typename ...> typename MFn>
        using apply = MFn<T...>;
    };

    template <template<typename ...> typename MFn, typename TList >
    using apply = typename TList::template apply<MFn>;


    template <typename TList>
    using typelist_variant_t = apply<std::variant, TList>;
    
    template <typename ...T>
    std::variant<T...> as_variant(TypeList<T...>);


    template <typename ...T>
    std::tuple<T...> as_tuple(TypeList<T...>);

    template <typename... Command>
    void list_trades(Command &&... args)
    {
        (args.describe(), ...);
    }

    template <typename List>
    typelist_variant_t<List> create_command(int choice)
    {
        auto result = typelist_variant_t<List>();
        switch (choice)
        {
        case 1:
            result = BuyStockCommand();
            break;
        case 2:
            result = SellStockCommand();
            break;
        case 3:
            result = ListAllStocksCommand();
            break;
        default:
            throw BadCommandException("Command not found");
        }

        return result;
    }

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


    template <typename Command>
    void place_trade(const Command& command)
    {
        command.execute();
    }

    template <typename List>
    void handle_command(typelist_variant_t<List> variant)
    {
        std::visit([](auto&& command)
        {
            using T = std::decay_t<decltype(command)>;
            if constexpr (hasExecute<T>)
                place_trade(command);
            else
                std::cout << "A command must implement execute" << std::endl;
        }, variant);

    }

} // namespace stock
