#ifndef LASER_H
#define LASER_H

#define _USE_MATH_DEFINES

#include "device.h"
#include "wave.h"

class Laser: public Device {
public:
    Laser(int id);  

    Wave getWave(int output = 0) const override;
    
    void setEx(std::complex<double> ex);
    std::complex<double> getEx();
    
    void setEy(std::complex<double> ey);
    std::complex<double> getEy();

private:
    std::complex<double> _ex, _ey;
    Wave _wave;
};

#endif // LASER_H
