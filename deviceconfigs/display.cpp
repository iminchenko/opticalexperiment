#include "display.h"
#include "devicedefines.h"
#include <QDebug>

using std::vector;

int64_t m(size_t i, size_t N) {
    if (!(N & 1)) {
        if (i > N / 2)
            return (int64_t)i - (int64_t)N / 2;
        else
            return - (int64_t)N / 2 + (int64_t)i - 1;
    }
    else {
       return -((int64_t)N + 1) / 2 + (int64_t)i;
    }
}

Display::Display(int id) :Device(deviceType::TYPE_SHIELD, id) {
    _connections.resize(1);
}

std::complex<double> Display::getValue(double x) const {
    return I(getWave(0), x);
}

std::complex<double> Display::I(const std::vector<Wave>& ws, double x) const {
    std::complex<double> Ix(0, 0), Iy(0, 0), a(0, 0);
    
    for (size_t k = 0; k < ws.size(); k++) {
        for (size_t p = 0; p < k; p++) {
            a = ws[k].getEx()*(std::conj(ws[p].getEx()));
            Ix += 2*(a.real()*cos((m(k + 1, ws.size()) - m(p + 1, ws.size()))*deltaK_*x)
                   - a.imag()*sin((m(k + 1, ws.size()) - m(p + 1, ws.size()))*deltaK_*x));

            a = ws[k].getEy()*(std::conj(ws[p].getEy()));
            Iy += 2*(a.real()*cos((m(k + 1, ws.size()) - m(p + 1, ws.size()))*deltaK_*x)
                    - a.imag()*sin((m(k + 1, ws.size()) - m(p + 1, ws.size()))*deltaK_*x));
        }

        Ix += ws[k].getEx()*(std::conj(ws[k].getEx()));
        Iy += ws[k].getEy()*(std::conj(ws[k].getEy()));
    }
    
    return Ix + Iy;
}

Waves Display::getWave(int) const {
    if (!_connections[0].device.expired())
        return _connections[0].device.lock()->getWave(_connections[0].output);

    return Waves();
}
