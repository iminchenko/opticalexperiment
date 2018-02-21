#pragma once

#include <complex>
#include <string>

using std::size_t;

template<typename T>
class Matrix;

class Device {
public:
    // потом убрать указатель
    Device(size_t inCount, size_t outCount, Matrix<std::complex<double>> *matr,
           std::string name, std::string description);

private:
    // потом убрать указатель
    Matrix<std::complex<double>> *_matrix;
    size_t _inCount;
    size_t _outCount;
    std::string _name;
    std::string _description;
};
