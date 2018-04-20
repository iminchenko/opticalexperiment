#define _USE_MATH_DEFINES
#include <cmath>

#include "laser.h"
#include "devicedefines.h"


Laser::Laser(int id) :
    Device(deviceType::TYPE_LASER, id)
    , _ex(48635.7, 48635.7)
    , _ey(48635.7, 48635.7)
    , _wave(_ex, _ey) 
{}

Waves Laser::getWave(int) const {
    _changed = false;
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

void Laser::setVariables(VarList vars) {
    for (const auto &iter : vars) {
        if (iter.first == "Ex") {
            _ex.real(iter.second * 48635.7);
        } else if (iter.first == "Exi") {
            _ex.imag(iter.second * 48635.7);
        } else if (iter.first == "Ey") {
            _ey.real(iter.second * 48635.7);
        } else if (iter.first == "Eyi") {
            _ey.imag(iter.second * 48635.7);
        }
    }
    _wave.setEx(_ex);
    _wave.setEy(_ey);

    _changed = true;
}
