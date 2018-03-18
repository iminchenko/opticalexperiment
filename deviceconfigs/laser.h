#ifndef LASER_H
#define LASER_H

#define _USE_MATH_DEFINES

#include "device.h"
#include "wave.h"

class Laser: public Device {
public:

    Laser(int id) :
         Device(deviceType::TYPE_LASER, id) ,
         _ex(0, 0), _ey(0, 0), _wave(_ex, _ey) {}  

    Wave getWave(int output = 0) const override { return  _wave; }
    
    void setEx(std::complex<double> ex) { _ex = ex; _wave.setEx(_ex); }
    std::complex<double> getEx() { return _ex; }
    
    void setEy(std::complex<double> ey) { _ey = ey; _wave.setEy(_ey); }
    std::complex<double> getEy() { return _ey; }

private:
    std::complex<double> _ex, _ey;
    Wave _wave;
};

#endif // LASER_H
