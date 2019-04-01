#include <stdexcept>

#include "wave.h"

Wave::Wave(): _ex(std::complex<double>(0., 0.)),
    _ey(std::complex<double>(0., 0.))
{}

Wave::Wave(const std::complex<double> &ex, const std::complex<double> &ey)
    : _ex(ex)
    , _ey(ey)
{}

Wave::Wave(double reEx, double imEx, double reEy, double imEy)
    : _ex(std::complex<double>(reEx, imEx))
    , _ey(std::complex<double>(reEy, imEy))
{}

const std::complex<double> &Wave::ex() const {
    return _ex;
}

void Wave::setEx(const std::complex<double> &ex)  {
    _ex = ex;
}

const std::complex<double> &Wave::ey() const {
    return _ey;
}

void Wave::setEy(const std::complex<double> &ey)  {
    _ey = ey; 
}

bool Wave::operator==(const Wave &wave) {
    return _ex == wave._ex && _ey == wave._ey;
}

Wave operator*(const TransMatrix &m, const Wave &w) {
    /* В волне всегда два компонента */
    if (m.columns() != 2) {
        throw std::logic_error("Количество столбцов матрицы не равно 2");
    }

    Wave newW;

    newW.setEx(m[0][0]*w.ex() + m[0][1]*w.ey());
    newW.setEy(m[1][0]*w.ex() + m[1][1]*w.ey());

    return newW;
}

Waves operator*(const TransMatrix &m, const Waves &ws) {
    if (m.columns() != ws.size() * 2) {
        throw std::logic_error("Умножение невозможно");
    }

    Waves newWs(static_cast<size_t>(m.rows() / 2));
    for (size_t i = 0; i < m.rows(); ++i) {
        std::complex<double> accum(0, 0);

        for (size_t j = 0; j < ws.size(); ++j) {
            accum += ws[j].ex() * m[i][2 * j];
            accum += ws[j].ey() * m[i][2 * j + 1];
        }

        if (i & 1) { // NOLINT
            newWs[i / 2].setEy(accum);
        } else {
            newWs[i / 2].setEx(accum);
        }
    }

    return newWs;
}

void clearEmpty(Waves &waves) {
    for (auto iter = waves.begin(); iter != waves.end();) {
        auto empty = Wave();
        if (*iter == empty) {
            iter = waves.erase(iter);
        } else {
            ++iter;
        }
    }
}
