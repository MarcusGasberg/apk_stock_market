//
// Created by stud on 12/14/20.
//

#ifndef BOOST_SIGNAL_EXAMPLE_PLACEHOLDERS_H
#define BOOST_SIGNAL_EXAMPLE_PLACEHOLDERS_H

#include "iostream"
#include "functional"

// Inspiration from "http://coliru.stacked-crooked.com/a/f9b21ba0b6b62e42"
template <int N>
struct custom_placeholder { static custom_placeholder cp; };

template<int N>
custom_placeholder<N> custom_placeholder<N>::cp;

namespace std {
    template<int N>
    struct is_placeholder<::custom_placeholder<N>> : std::integral_constant<int, N> { };
}

// TODO: Use boost::ref or boost::cref?
// TODO: Problems with this reference? Should be forwarded or something?
template<class R, class T, class...Types,  int... indices>
std::function<R (Types...)> custom_bind(R (T::* providerCallback)(Types ...), T * providerObject, std::integer_sequence<int, indices...>)
{
    return std::bind(providerCallback, providerObject, custom_placeholder<indices+1>::cp...);
}

template<class R, class T, class...Types>
std::function<R (Types...)> custom_bind(R (T::* providerCallback)(Types ...), T * providerObject) {
    return custom_bind(providerCallback, providerObject, std::make_integer_sequence<int, sizeof...(Types)>());
}


#endif //BOOST_SIGNAL_EXAMPLE_PLACEHOLDERS_H
