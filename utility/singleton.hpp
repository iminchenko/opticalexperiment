#pragma once

// TODO: интегрировать нормальный синглтон вместо этого
template<class T>
T &singleton() {
    static T object;
    return object;
}
