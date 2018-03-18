#include <cmath>

#include "laser.h"
#include "devicedefines.h"

#define _USE_MATH_DEFINES

Laser::Laser(int id)
    :Device(deviceType::TYPE_LASER, id) {}

void Laser::setLambda(const double lambda) {
    lambda_ = lambda;
}

double Laser::getLamda() const {
    return lambda_;
}

double Laser::getK() const {
    return 2*M_PI / lambda_;
}

double Laser::getOmega() const {
    return 2*M_PI*c / lambda_;
}

Wave Laser::getWave(int /*out*/) const {
    return Wave();
}
