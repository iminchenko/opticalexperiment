#include "diffractiongrating.h"

DiffractionGrating::DiffractionGrating(int id)
    : Display (id)
{
    setType(TYPE_DIFFRACTION_GRATING);
}

double DiffractionGrating::getA() {
    return _a;
}

double DiffractionGrating::getB() {
    return _b;
}

double DiffractionGrating::getW() {
    return _W;
}

double DiffractionGrating::getN3() {
    return _N3;
}

void DiffractionGrating::setVariables(VarList vars) {
    Display::setVariables(vars);
    for (const auto& iter : vars) {
        if (iter.first == "b") {
            _a = iter.second;
        } else if (iter.first == "a") {
            _b = iter.second;
        } else if (iter.first == "W") {
            _W = iter.second;
        } else if (iter.first == "N3") {
            _N3 = iter.second;
        }
    }
}
