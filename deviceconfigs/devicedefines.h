#pragma once

#include <complex>

template<typename T>
class Matrix;

// матрица перехода
using TransMatrix = Matrix<std::complex<double>>;

// Захардкоженные типы
enum deviceType {
    TYPE_LASER = -1,
    TYPE_SHIELD = -2,
    TYPE_GENERIC = 0
};
