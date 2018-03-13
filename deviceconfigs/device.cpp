#include "device.h"

Device::Device(size_t inCount, size_t outCount,
               const Matrix<std::complex<double> > &matr)
    :_matrix(matr), _inCount(inCount), _outCount(outCount) {}

const Matrix<std::complex<double> > &Device::getMatrix() const {
    return _matrix;
}
