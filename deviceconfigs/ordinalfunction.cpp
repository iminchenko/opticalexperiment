#include "ordinalfunction.h"

QVector<size_t> OrdinalFunction::path() {
    createNotUsedNode(_inc.rows());
    createHierarchyLvls();

    QVector<size_t> qv;
    for (int i = 0; i < _highLvls.size(); ++i)
        for (int j = 0; j < _highLvls[i].size(); ++j)
            qv.append(_highLvls[i][j]);

    allClear();
    return std::move(qv);
}

QVector<size_t> OrdinalFunction::path(const Matrix<size_t> &inc) {
    _inc = inc;
    return path();
}

void OrdinalFunction::createNullLlv() {
    _highLvls.push_back(QVector<size_t>());
    bool check;
    
    for (size_t column = 0; column < _inc.columns(); ++column)
    {
        check = true;
        for (size_t row = 0; row < _inc.rows(); ++row) {
            if (_inc[row][column] != 0) {
                check = false;
                break;
            }
        }

        if (check) {
            _highLvls[0].append(column);
            _notUsedNode.removeOne(column);
        }
    }
}

void OrdinalFunction::createNotUsedNode(size_t N) {
    for (size_t i = 0; i < N; ++i)
        _notUsedNode.push_back(i);
}

void OrdinalFunction::checkOnCoherency(size_t lvl) {
    int swap;
    for (int i = 0; i < _highLvls[lvl].size() - 1; ++i)
        for (int j = i + 1; j < _highLvls[lvl].size(); ++j)
            if (_inc[_highLvls[lvl][i]][_highLvls[lvl][j]] != 0) {
                swap = _highLvls[lvl][j];
                _highLvls[lvl][j] = _highLvls[lvl][i];
                _highLvls[lvl][i] = swap;
            }
}

void OrdinalFunction::createHierarchyLvls() {
    createNullLlv();
    if (_highLvls[0].size() == 0)
        throw "Не удалось создать первый уровень";

    size_t currLvl = 1, row = 0;
    int  indexNode = 0;
    
    while(_notUsedNode.size() != 0) {
        _highLvls.append(QVector<size_t>());
        for (int count = 0; count < _highLvls[currLvl - 1].size(); ++row) {
            row = _highLvls[currLvl - 1][count];
            for (size_t column = 0; column < _inc.columns(); ++column)
                if (_inc[row][column] != 0
                        && (indexNode = _notUsedNode.indexOf(column)) != -1) {
                    _notUsedNode.removeAt(indexNode);
                    _highLvls[currLvl].append(column);
                }
        }
        
        checkOnCoherency(currLvl);
        ++currLvl;
    }    
}

void OrdinalFunction::allClear() {
    _highLvls.clear();
    _notUsedNode.clear();
}


