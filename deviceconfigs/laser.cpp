#define _USE_MATH_DEFINES
#include <cmath>

#include "laser.h"
#include "devicedefines.h"


Laser::Laser(int id) :
    Device(deviceType::TYPE_LASER, id)
    , _ex(1, 1)
    , _ey(1, 1)
    , _wave(_ex, _ey) 
{}

Waves Laser::getWave(int) const {
    return Waves{_wave};
}

void Laser::setEx(std::complex<double> ex) {
    _ex = ex; 
    _wave.setEx(_ex);
}

std::complex<double> Laser::getEx() {
    return _ex; 
}

void Laser::setEy(std::complex<double> ey) {
    _ey = ey; 
    _wave.setEy(_ey); 
}

std::complex<double> Laser::getEy() {
    return _ey; 
}
