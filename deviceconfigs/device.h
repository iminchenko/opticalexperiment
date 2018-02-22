#pragma once

#include <complex>
#include <string>

using std::size_t;

template<typename T>
class Matrix;

class Device {
public:
    // чтобы вектор не ругался
    Device() = default;
    // потом убрать указатель
    Device(size_t inCount, size_t outCount, Matrix<std::complex<double>> *matr);

private:
    // потом убрать указатель
    Matrix<std::complex<double>> *_matrix;
    // а нужны ли эти параметры?
    size_t _inCount;
    size_t _outCount;
};
