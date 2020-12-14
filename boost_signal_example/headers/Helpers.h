//
// Created by stud on 12/14/20.
//

#ifndef BOOST_SIGNAL_EXAMPLE_HELPERS_H
#define BOOST_SIGNAL_EXAMPLE_HELPERS_H
#include <type_traits>

template<typename T>
struct HasEqualityHelper
{
    template<typename C, typename = decltype(std::declval<C>().operator==(std::declval<C>()))>
    static std::true_type test(void*);

    // test() fallback:
    template<typename>
    static std::false_type test(...);

    using Type = decltype(test<T>(nullptr));
    static constexpr bool value = std::is_same_v<decltype(test<T>(nullptr)), char>;
};

template<typename T>
struct HasEquality : HasEqualityHelper<T>::Type {};
#endif //BOOST_SIGNAL_EXAMPLE_HELPERS_H
