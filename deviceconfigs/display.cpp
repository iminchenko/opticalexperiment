#include "display.h"
#include "devicedefines.h"
#include <QDebug>

using std::vector;

//template <typename T1, typename  T2>
//T1 m(T1 i, T2 N) {
//    if ((T1)N % 2 == 0) {
//        if (i > N / 2)
//            return i - (T1)N / 2;
//        else
//            return -(T1)N / 2 + i - 1;
//    }
//    else {
//        return -((T1)N + 1) / 2 + i;
//    }
//}

int Display::m(int i, int N) const {
    if (N % 2 == 0) {
        if (i > N / 2)
            return i - N / 2;
        else
            return - N / 2 + i - 1;
    }
    else {
       return -(N + 1) / 2 + i;
    }

}

Display::Display(int id) :Device(deviceType::TYPE_SHIELD, id) {}

std::complex<double> Display::getValue(double x) const {
    return I(getWave(0), x);
}

std::complex<double> Display::I(const std::vector<Wave>& ws, double x) const {
    std::complex<double> Ix(0, 0), Iy(0, 0), a(0, 0);
    
//    for (int i = 1; i <= 4; i++)
//        qDebug() << QString("i = %1, N = 4, m = %2").arg(i).arg(m(i, 4));

//    return std::complex<double>();
    //qDebug() << QString("x = %1").arg(x);
    //qDebug() << QString("deltaK = %1").arg(deltaK_);
    for (unsigned int k = 0; k < ws.size(); k++) {
        for (unsigned int p = 0; p < k; p++) {
            //qDebug() << QString("k = %1, p = %2").arg(k + 1).arg(p + 1);
            a = ws[k].getEx()*(std::conj(ws[p].getEx()));
            //qDebug() << QString("A: Re = %1, Im = %2").arg(a.real()).arg(a.imag());
            qDebug() << QString("k = %1, p = %2, mk = %3, mp = %4").arg(k + 1).arg(p + 1).arg(m(k + 1, ws.size())).arg(m(p + 1, ws.size()));
            Ix += 2*(a.real()*cos((m(k + 1, ws.size()) - m(p + 1, ws.size()))*deltaK_*x)
                   - a.imag()*sin((m(k + 1, ws.size()) - m(p + 1, ws.size()))*deltaK_*x));

            a = ws[k].getEy()*(std::conj(ws[p].getEy()));
            //qDebug() << QString("A: Re = %1, Im = %2").arg(a.real()).arg(a.imag());
            Iy += 2*(a.real()*cos((m(k + 1, ws.size()) - m(p + 1, ws.size()))*deltaK_*x)
                    - a.imag()*sin((m(k + 1, ws.size()) - m(p + 1, ws.size()))*deltaK_*x));
        }

        Ix += ws[k].getEx()*(std::conj(ws[k].getEx()));
        Iy += ws[k].getEy()*(std::conj(ws[k].getEy()));
    }
    
    //qDebug() << QString("I = %1").arg((Ix + Iy).real());
    qDebug() << "-------------------";
    return Ix + Iy;
}

Waves Display::getWave(int) const {
    if (!_connections[0].device.expired())
        return _connections[0].device.lock()->getWave(_connections[0].output);

    return Waves();
}
