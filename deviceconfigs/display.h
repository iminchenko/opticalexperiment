#ifndef DISPLAY_H
#define DISPLAY_H
#include "matrix.h"
#include "wave.h"
#include <cmath>

class Display
{
public:
    Display();
    std::complex<double> I(const std::vector<Wave>& ws, double x);

private:
    double K_ = 2*M_PI;
    double deltaK_ = 1e-2*K_;
    int m(int i, int N);
};
#endif // DISPLAY_H
