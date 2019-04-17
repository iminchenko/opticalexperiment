#pragma once

#include "wave.h"
#include "deviceconfigs/display.h"
#include <math.h>

class DiffractionGrating: public Display {
public:
    explicit DiffractionGrating(int id);

    void setVariables(VarList vars) override ;

    double getA();
    double getB();
    double getW();
    double getN3();

private:
    double _a = 0.00001;
    double _b = 0.000001;
    double _W = 1;
    double _N3 = 1;
};

