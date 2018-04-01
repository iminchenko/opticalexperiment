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

std::complex<double> Wave::getEx() const {
    return _ex;
}

void Wave::setEx(const std::complex<double> &ex)  {
    _ex = ex;
}

std::complex<double> Wave::getEy() const {
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
    if (m.getColumns() != 2)
        throw "Количество столбцов матрицы не равно 2";

    Wave newW;

    newW.setEx(m[0][0]*w.getEx() + m[0][1]*w.getEy());
    newW.setEy(m[1][0]*w.getEx() + m[1][1]*w.getEy());

    return newW;
}

Waves operator*(const TransMatrix &m, const Waves &ws) {
    if (m.getColumns() != ws.size() * 2)
        throw "Умножение не возможно";

    Waves newWs((size_t)(m.getRows() / 2));
    for (size_t i = 0; i < m.getRows(); ++i) {
        std::complex<double> accum(0, 0);

        for (int j = 0; j < ws.size(); ++j) {
            accum += ws[j].getEx() * m[i][2 * j];
            accum += ws[j].getEy() * m[i][2 * j + 1];
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

