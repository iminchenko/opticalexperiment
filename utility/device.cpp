#include "device.h"

Device::Device(size_t inCount, size_t outCount,
  Matrix<std::complex<double>> *matr, std::string name, std::string description)
    :_inCount(inCount), _outCount(outCount), _matrix(matr), _name(name),
    _description(description) {}
