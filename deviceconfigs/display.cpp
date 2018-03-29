#include "display.h"
#include "devicedefines.h"

using std::vector;

template <typename T1, typename  T2>
T1 m(T1 i, T2 N) {
    if ((T1)N / 2 == 0) {
        if (i > N)
            return i - (T1)N / 2;
        else
            return -(T1)N / 2 + i - 1;
    }
    else {
        return -((T1)N + 2) / 2 + i;
    }

}

Display::Display(int id) :Device(deviceType::TYPE_SHIELD, id) {}

std::complex<double> Display::getValue(double x) const {
    return I(vector<Wave>{getWave(0)}, x);
}

std::complex<double> Display::I(const std::vector<Wave>& ws, double x) const {
    std::complex<double> Ix(0, 0), Iy(0, 0), a(0, 0);

    for (unsigned int k = 0; k < ws.size(); k++) {
        for (unsigned int p = 0; p < k; p++) {
            a = ws[k].getEx()*(std::conj(ws[p].getEx()));
            Ix += 2*(a.real()*cos(m(k + 1, ws.size()) - m(p + 1, ws.size()))*deltaK_*x
                   - a.imag()*sin(m(k + 1, ws.size()) - m(p + 1, ws.size()))*deltaK_*x);

            a = ws[k].getEy()*(std::conj(ws[p].getEy()));
            Iy += 2*(a.real()*cos(m(k + 1, ws.size()) - m(p + 1, ws.size()))*deltaK_*x
                    - a.imag()*sin(m(k + 1, ws.size()) - m(p + 1, ws.size()))*deltaK_*x);
        }

        Ix += ws[k].getEx()*(std::conj(ws[k].getEx()));
        Iy += ws[k].getEy()*(std::conj(ws[k].getEy()));
    }

    return Ix + Iy;
}

Wave Display::getWave(int) const {
    if (_connections[0].device)
        return _connections[0].device->getWave(_connections[0].output);

    return Wave();
}
