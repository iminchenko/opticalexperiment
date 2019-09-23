#pragma once

#include "deviceconfigs/display.h"
#include "wave.h"
#include <cmath>

class DiffractionGrating : public Display {
public:
    explicit DiffractionGrating(int id);

    void setVariables(VarList vars) override;

    double getA() const;
    double getB() const;
    double getW() const;
    double getN3() const;

private:
    double _a  = 0.00001;
    double _b  = 0.000001;
    double _W  = 1;
    double _N3 = 1;
};
