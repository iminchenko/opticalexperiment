#pragma once

#include <utility>

template<typename T1, typename T2, typename T3>
struct triple {
    T1 first;
    T2 second;
    T3 third;

    triple(T1 t1, T2 t2, T3 t3) :first(t1), second(t2), third(t3) {}
//    triple &operator=(const triple&) = default;
};

template<typename T1, typename T2, typename T3>
constexpr triple<T1, T2, T3> make_triple(T1 t1, T2 t2, T3 t3) {
    return triple<T1, T2, T3>(t1, t2, t3);
}
