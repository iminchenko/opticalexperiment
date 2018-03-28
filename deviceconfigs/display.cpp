#include "display.h"
#include "devicedefines.h"

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

std::complex<double> Display::I(const std::vector<Wave>& ws, double x) {
    std::complex<double> Ix(0, 0), Iy(0, 0), a(0, 0);

    for (unsigned int k = 1; k < ws.size(); k++) {
        for (unsigned int p = 1; p < k; p++) {
            a = ws[k - 1].getEx()*(std::conj(ws[p - 1].getEx()));
            Ix += 2*(a.real()*cos(m(k, ws.size()) - m(p, ws.size()))*deltaK_*x
                   - a.imag()*sin(m(k, ws.size()) - m(p, ws.size()))*deltaK_*x);

            a = ws[k - 1].getEy()*(std::conj(ws[p - 1].getEy()));
            Iy += 2*(a.real()*cos(m(k, ws.size()) - m(p, ws.size()))*deltaK_*x
                    - a.imag()*sin(m(k,ws.size()) - m(p, ws.size()))*deltaK_*x);
        }

        Ix += ws[k - 1].getEx()*(std::conj(ws[k - 1].getEx()));
        Iy += ws[k - 1].getEy()*(std::conj(ws[k - 1].getEy()));
    }

    return Ix + Iy;
}

Wave Display::getWave(int) const {
    return Wave();
}
