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

Wave operator*(const Matrix<std::complex<double>> &m, const Wave &w) {
    /* В волне всегда два компонента */
    if (m.getColumns() != 2)
        throw "Количество столбцов матрицы не равно 2";

    Wave newW;

    newW.setEx(m[0][0]*w.getEx() + m[0][1]*w.getEy());
    newW.setEy(m[1][0]*w.getEx() + m[1][1]*w.getEy());

    return newW;
}

std::vector<Wave> operator*(const Matrix<std::complex<double>> &m,
                            const std::vector<Wave> &ws) {
    if (m.getColumns() != ws.size()*2)
        throw "Умножение не возможно";

    std::complex<double> accum(0, 0);
    std::vector<Wave> newWs(ws.size());
    for (size_t i = 0; i < m.getColumns(); ++i) {
        for (int j = 0; j < m.getRows(); j += 2) {
            accum += ws.at(i / 2).getEx() * m[i][j];
            accum += ws.at(i / 2).getEy() * m[i][j + 1];
        }

        (i % 2 == 0 ? newWs.at(i / 2).setEx(accum)
                    : newWs.at(i / 2).setEy(accum));
        accum = 0;
    }
}
