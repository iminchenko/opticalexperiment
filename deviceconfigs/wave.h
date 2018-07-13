#pragma once

#include <complex>

#include "matrix.h"
#include "devicedefines.h"

class Wave;
using Waves = std::vector<Wave>;

class Wave {
public:
    Wave();
    Wave(const std::complex<double> &ex, const std::complex<double> &ey);
    Wave(double reEx, double imEx, double reEy, double imEy);

    const std::complex<double> &ex() const;
    void setEx(const std::complex<double> &ex);

    const std::complex<double> &ey() const;
    void setEy(const std::complex<double> &ey);

    bool operator==(const Wave &wave);

private:
    std::complex<double> _ex;
    std::complex<double> _ey;
};

Wave operator*(const TransMatrix &m, const Wave &w);

Waves operator*(const TransMatrix &m, const Waves &ws);

void clearEmpty(Waves &waves);

