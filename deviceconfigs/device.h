#pragma once

#include <complex>
#include <string>

#include "matrix.h"

using std::size_t;

class Device {
public:
    // чтобы вектор не ругался
    Device() = default;
    // потом убрать указатель
    Device(size_t inCount, size_t outCount,
           const Matrix<std::complex<double>> &matr);

    const Matrix<std::complex<double>> &getMatrix() const;

private:
    Matrix<std::complex<double> > _matrix;
    // а нужны ли эти параметры?
    size_t _inCount;
    size_t _outCount;
};
