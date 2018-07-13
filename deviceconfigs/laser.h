#pragma  once

#include <QObject>

#include "device.h"
#include "wave.h"


class Laser: public Device {
public:
    explicit Laser(int id);

    Waves getWave(int output) const override;
    
    void setEx(std::complex<double> ex);
    const std::complex<double> &ex() const;
    
    void setEy(std::complex<double> ey);
    const std::complex<double> &ey() const;

    void setVariables(VarList vars) override;

private:
    std::complex<double> _ex;
    std::complex<double> _ey;
    Wave _wave;
};
