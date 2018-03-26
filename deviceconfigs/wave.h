#ifndef WAVE_H
#define WAVE_H

#include <complex>
#include "matrix.h"

class Wave
{
public:
    Wave();
    Wave(const std::complex<double> &ex, const std::complex<double> &ey);
    Wave(double reEx, double imEx, double reEy, double imEy);

    std::complex<double> getEx() const;
    void setEx(const std::complex<double> &ex);

    std::complex<double> getEy() const;
    void setEy(const std::complex<double> &ey);

private:
    std::complex<double> _ex;
    std::complex<double> _ey;
};

template<class T>
Wave operator *(const Matrix<std::complex<double>> &m, const Wave &w)
{
    /* В волне всегда два компонента */
    if (m.getColumns() != 2)
        throw "Количество столбцов матрицы не равно 2";

    Wave newW;

    newW.setEx(m[0][0]*w.getEx() + m[0][1]*w.getEy());
    newW.setEy(m[1][0]*w.getEx() + m[1][1]*w.getEy());

    return newW;
}

template<class T>
std::vector<Wave> operator *(const Matrix<std::complex<double>> &m, 
                             const std::vector<Wave> &ws)
{
    if (m.getColumns() != ws.size()*2)
        throw "Умножение не возможно";
    
    std::complex<double> accum = 0;
    std::vector<Wave> newWs(ws.size());
    for (int i = 0; i < m.getColumns(); ++i) {
        for (int j = 0; j < m.getRows(); j += 2) {
            accum += ws.at(i / 2).getEx() * m[i][j];
            accum += ws.at(i / 2).getEy() * m[i][j + 1];
        }
        
        (i % 2 == 0 ? newWs.at(i / 2).setEx(accum) 
                   : newWs.at(i / 2).setEy(accum));
        accum = 0;
    }
}

#endif // WAVE_H
