#pragma once

#include <complex>
#include "matrix.h"

class Wave {
public:
    Wave();
    Wave(const std::complex<double> &ex, const std::complex<double> &ey);
    Wave(double reEx, double imEx, double reEy, double imEy);

    std::complex<double> getEx() const;
    void setEx(const std::complex<double> &ex);

    std::complex<double> getEy() const;
    void setEy(const std::complex<double> &ey);

private:
    std::complex<double> _ex;
    std::complex<double> _ey;
};

Wave operator*(const Matrix<std::complex<double>> &m, const Wave &w);

std::vector<Wave> operator*(const Matrix<std::complex<double>> &m,
                             const std::vector<Wave> &ws);

