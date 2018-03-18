#ifndef LASER_H
#define LASER_H

#include "device.h"

class Laser: public Device {
public:
    Laser(int id);
    static const double c;

    void setLambda(const double lambda);
    double getLamda() const;

    double getK() const; // k = 2*pi/lambda
    double getOmega() const; // omega = 2*pi*c/lambda

    Wave getWave(int output) const override;

private:
    double lambda_ = 1;
};

const double Laser::c = 1;

#endif // LASER_H
