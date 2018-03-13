#include "device.h"

Device::Device(size_t inCount, size_t outCount,
                                        Matrix<std::complex<double>> *matr)
    :_matrix(matr), _inCount(inCount), _outCount(outCount) {}
