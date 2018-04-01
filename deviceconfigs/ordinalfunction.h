#pragma once

#include "matrix.h"
#include <QVector>

class OrdinalFunction {
public:
    OrdinalFunction() = default;

    QVector<int> path();
    QVector<int> path(const Matrix<int> &inc);

private:
    Matrix<int> inc_;
    QVector<int> notUsedNode_;
    QVector<QVector<int>> highLvls_;

    void createNullLlv();
    void deleteUsedNode(const int lvl);
    void createNotUsedNode(const int N);
    void checkOnCoherency(const int lvl);
    void createHierarchyLvls();
    void allClear(); //Зачистка всех векторов
};
