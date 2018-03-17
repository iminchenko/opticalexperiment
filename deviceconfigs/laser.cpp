#include "laser.h"

Laser::Laser()
{

}

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
