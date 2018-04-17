#pragma  once

#include <cmath>

#include "wave.h"
#include "device.h"

class Display: public Device {
public:
    explicit Display(int id);

    std::complex<double> getValue(double x) const;

    Waves getWave(int output) const override;

private:
    std::complex<double> I(const std::vector<Wave>& ws, double x) const;

    double K_ = M_PI / 2;
    double deltaK_ = 1*K_;
};
