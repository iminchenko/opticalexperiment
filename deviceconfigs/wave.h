#ifndef WAVE_H
#define WAVE_H

#include <complex>

class Wave
{
public:
    Wave(): ex_(std::complex<double>(0., 0.)), ey_(std::complex<double>(0., 0.))
    {}
    Wave(std::complex<double> ex, std::complex<double> ey): ex_(ex), ey_(ey)
    {}
    Wave(double reEx, double imEx, double reEy, double imEy):
        ex_(std::complex<double>(reEx, imEx)), ey_(std::complex<double>(reEy, imEy))
    {}

    std::complex<double> getEx() const {
        return ex_;
    }
    void setEx(const std::complex<double>& ex) {
        ex_ = ex;
    }

    std::complex<double> getEy() const {
        return ey_;
    }
    void setEy(std::complex<double>& ey) {
        ey_ = ey;
    }

private:
    std::complex<double> ex_;
    std::complex<double> ey_;
};

#endif // WAVE_H
