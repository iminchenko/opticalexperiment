#include "ordinalfunction.h"

OrdinalFunction::OrdinalFunction() {
    /* Test matrix
    Matrix<int> inc(5, 5, 0);

    inc_.setMatrix(inc.getMatrix());

    inc_[0][3] = 1;
    inc_[1][2] = 1;
    inc_[3][2] = 1;
    inc_[2][4] = 1; */
}

QVector<int> OrdinalFunction::path() {
    createNotUsedNode(inc_.getRows());
    createHierarchyLvls();

    QVector<int> qv;
    for (int i = 0; i < highLvls_.size(); i++)
        for (int j = 0; j < highLvls_[i].size(); j++)
            qv.append(highLvls_[i][j]);

    allClear();
    return std::move(qv);
}

QVector<int> OrdinalFunction::path(const Matrix<int> &inc) {
    inc_ = inc;
    return path();
}

void OrdinalFunction::createNullLlv() {
    int row, column;
    highLvls_.push_back(QVector<int>());
    bool check;
    for (column = 0; column < inc_.getColumns(); column++)
    {
        check = true;
        for (row = 0; row < inc_.getRows(); row++) {
            if (inc_[row][column] != 0) {
                check = false;
                break;
            }
        }

        if (check) {
            highLvls_[0].append(column);
            notUsedNode_.removeOne(column);
        }
    }
}

void OrdinalFunction::createNotUsedNode(const int N) {
    for (int i = 0; i < N; i++)
        notUsedNode_.push_back(i);
}

void OrdinalFunction::checkOnCoherency(const int lvl) {
    int swap;
    for (int i = 0; i < highLvls_[lvl].size() - 1; i++)
        for (int j = i + 1; j < highLvls_[lvl].size(); j++)
            if (inc_[highLvls_[lvl][i]][highLvls_[lvl][j]] != 0) {
                swap = highLvls_[lvl][j];
                highLvls_[lvl][j] = highLvls_[lvl][i];
                highLvls_[lvl][i] = swap;
            }
}

void OrdinalFunction::createHierarchyLvls() {
    int currLvl = 1, indexNode, count, column, row;

    createNullLlv();
    if (highLvls_[0].size() == 0)
        throw "Не удалось создать первый уровень";

    while(notUsedNode_.size() != 0) {
        highLvls_.append(QVector<int>());
        for (count = 0; count < highLvls_[currLvl - 1].size(); count++) {
            row = highLvls_[currLvl - 1][count];
            for (column = 0; column < inc_.getColumns(); column++)
                if (inc_[row][column] != 0
                        && (indexNode = notUsedNode_.indexOf(column)) != -1) {
                    notUsedNode_.removeAt(indexNode);
                    highLvls_[currLvl].append(column);
                }
        }
        
        checkOnCoherency(currLvl);
        currLvl++;
    }    
}

void OrdinalFunction::allClear() {
    highLvls_.clear();
    notUsedNode_.clear();
}


