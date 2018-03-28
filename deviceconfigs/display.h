#pragma  once

#include <cmath>

#include "wave.h"
#include "device.h"

class Display: public Device {
public:
    explicit Display(int id);
    std::complex<double> I(const std::vector<Wave>& ws, double x);

private:
    Wave getWave(int) const override;

    double K_ = 2*M_PI;
    double deltaK_ = 1e-2*K_;
};
