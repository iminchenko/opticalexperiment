#pragma once

#include <complex>
#include <list>
#include <string>
#include <utility>

template<typename T>
class Matrix;

// матрица перехода
using TransMatrix = Matrix<std::complex<double>>;
// матрица со строковыми выражениями
using ExprMatrix = Matrix<std::pair<std::string, std::string>>;
// список переменных
using VarList = std::list<std::pair<std::string, double>>;

// Захардкоженные типы
enum deviceType {
    TYPE_LASER = -1,
    TYPE_SHIELD = -2,
    TYPE_DIFFRACTION_GRATING = -3,
    TYPE_GENERIC = 0
};
