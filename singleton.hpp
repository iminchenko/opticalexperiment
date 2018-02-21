#pragma once

template<class T>
T &singleton() {
    static T object;
    return object;
}
