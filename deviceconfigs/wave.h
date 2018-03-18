#ifndef WAVE_H
#define WAVE_H

#include <complex>

class Wave
{
public:
    Wave(): _ex(std::complex<double>(0., 0.)), _ey(std::complex<double>(0., 0.))
    {}
    Wave(std::complex<double> ex, std::complex<double> ey): _ex(ex), _ey(ey)
    {}
    Wave(double reEx, double imEx, double reEy, double imEy):
        _ex(std::complex<double>(reEx, imEx)), _ey(std::complex<double>(reEy, imEy))
    {}

    std::complex<double> getEx() const { return _ex; }
    void setEx(const std::complex<double>& ex) { _ex = ex; }

    std::complex<double> getEy() const { return _ey; }
    void setEy(std::complex<double>& ey) { _ey = ey; }

private:
    std::complex<double> _ex;
    std::complex<double> _ey;
};

#endif // WAVE_H
