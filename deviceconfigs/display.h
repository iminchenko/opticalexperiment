#ifndef DISPLAY_H
#define DISPLAY_H
#include <cmath>

#include "wave.h"
#include "device.h"

class Display: public Device {
public:
    Display(int id);
    std::complex<double> I(const std::vector<Wave>& ws, double x);

private:
    Wave getWave(int) const override;

    double K_ = 2*M_PI;
    double deltaK_ = 1e-2*K_;
    int m(int i, int N);
};
#endif // DISPLAY_H
