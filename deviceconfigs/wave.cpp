#include "wave.h"

Wave::Wave(): _ex(std::complex<double>(0., 0.)),
    _ey(std::complex<double>(0., 0.))
{}

Wave::Wave(const std::complex<double> &ex, const std::complex<double> &ey)
    : _ex(ex)
    , _ey(ey)
{}

Wave::Wave(double reEx, double imEx, double reEy, double imEy)
    : _ex(std::complex<double>(reEx, imEx))
    , _ey(std::complex<double>(reEy, imEy))
{}

std::complex<double> Wave::getEx() const
{ 
    return _ex;
}

void Wave::setEx(const std::complex<double> &ex) 
{ 
    _ex = ex;
}

std::complex<double> Wave::getEy() const 
{
    return _ey;
}

void Wave::setEy(const std::complex<double> &ey) 
{ 
    _ey = ey; 
}
