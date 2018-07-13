#pragma once

#include "matrix.h"
#include <QVector>

class OrdinalFunction {
public:
    OrdinalFunction() = default;

    QVector<size_t> path();
    QVector<size_t> path(const Matrix<size_t> &inc);

private:
    void createNullLlv();
    void deleteUsedNode(size_t lvl);
    void createNotUsedNode(size_t N);
    void checkOnCoherency(size_t lvl);
    void createHierarchyLvls();
    void allClear();                        //Зачистка всех векторов
    
private:
    Matrix<size_t> _inc;
    QVector<size_t> _notUsedNode;
    QVector<QVector<size_t>> _highLvls;
};
