#pragma  once

#include <QObject>

#include "device.h"
#include "wave.h"


class Laser: public Device {
public:
    explicit Laser(int id);

    Wave getWave(int output) const override;
    
    void setEx(std::complex<double> ex);
    std::complex<double> getEx();
    
    void setEy(std::complex<double> ey);
    std::complex<double> getEy();

private:
    std::complex<double> _ex, _ey;
    Wave _wave;
};
