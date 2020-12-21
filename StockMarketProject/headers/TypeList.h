#pragma once
#include <tuple>
#include <variant>

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
}
