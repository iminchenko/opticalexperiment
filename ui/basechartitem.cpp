#include "basechartitem.h"

BaseChartItem::~BaseChartItem() {
    delete _3dDataArray;
    delete _2dDataArray;
}

QSurfaceDataArray *BaseChartItem::getDefault3dChart() {
    auto dataArray = new QSurfaceDataArray;
    auto newRow    = new QSurfaceDataRow();

    newRow->push_back(QSurfaceDataItem(
        QVector3D(static_cast<float>(_minX), 0, static_cast<float>(_minY))));
    newRow->push_back(QSurfaceDataItem(
        QVector3D(static_cast<float>(_maxX), 0, static_cast<float>(_minY))));
    *dataArray << newRow;

    newRow = new QSurfaceDataRow();

    newRow->push_back(QSurfaceDataItem(
        QVector3D(static_cast<float>(_minX), 0, static_cast<float>(_maxY))));
    newRow->push_back(QSurfaceDataItem(
        QVector3D(static_cast<float>(_maxX), 0, static_cast<float>(_maxY))));
    *dataArray << newRow;

    return dataArray;
}

QXYSeries *BaseChartItem::getDefault2dChart() {
    QXYSeries *series = new QLineSeries();

    series->append(_min2dX, 0);
    series->append(_max2dX, 0);

    return series;
}

QSurfaceDataArray *BaseChartItem::get3dSeries() {
    auto series = new QSurfaceDataArray();
    if (!_3dDataArray) {
        _3dDataArray = fill3DSeries();
    }

    for (int i = 0; i < _3dDataArray->size(); i++) {
        auto *newRow = new QSurfaceDataRow();
        for (int j = 0; j < _3dDataArray->at(i)->size(); j++) {
            newRow->push_back(_3dDataArray->at(i)->at(j));
        }
        series->append(newRow);
    }
    return series;
}

QXYSeries *BaseChartItem::get2dSeries() {
    QXYSeries *series = new QLineSeries();
    if (!_2dDataArray) {
        _2dDataArray = fill2DSeries();
    }
    // copying
    for (int i = 0; i < _2dDataArray->count(); i++) {
        series->append(_2dDataArray->at(i));
    }
    return series;
}

double BaseChartItem::randomDouble(double max) const {
    return static_cast<double>((qrand()) / RAND_MAX * max);
}

std::vector<QPointF> BaseChartItem::getSourcesPosition(size_t sourcesNum) {
    std::vector<QPointF> sources = std::vector<QPointF>();
    if (_sourcePositionMode == SourcePositionMode::InCircle) {
        if (_sourcesForRand.size() < sourcesNum) {
            for (size_t i = _sourcesForRand.size(); i < sourcesNum; i++) {
                _sourcesForRand.push_back(
                    getSourcePosition(_sourcePositionMode, i, sourcesNum));
            }
        }
        for (size_t i = 0; i < sourcesNum; i++) {
            sources.push_back(_sourcesForRand[i]);
        }
    } else {
        for (size_t i = 0; i < sourcesNum; i++) {
            sources.push_back(
                getSourcePosition(_sourcePositionMode, i, sourcesNum));
        }
    }
    return sources;
}

QPointF BaseChartItem::getSourcePosition(SourcePositionMode mod,
                                         size_t sourceId,
                                         size_t sourceCount) const {
    // parity of sources number
    bool parity = !(sourceCount & 1);

    switch (mod) {
    case SourcePositionMode::OnlyX:
        return getSourcePositionX(sourceId, parity);
    case SourcePositionMode::OnlyY:
        return getSourcePositionY(sourceId, parity);
    case SourcePositionMode::OnCircle:
        return getSourcePositionOnCircle(sourceId, sourceCount);
    case SourcePositionMode::InCircle:
        return getSourcePositionInCircle();
    }
    // default value
    return getSourcePositionX(sourceId, parity);
}

QPointF BaseChartItem::getSourcePositionY(size_t sourceId, bool parity) const {
    QPointF sourceCoord(0, 0);
    if (!parity) {
        // sequence for even number of sources
        sourceCoord.ry() = (-(0.5 + (sourceId << 1))) * MATH_D;
    } else {
        // sequence for odd number of sources
        sourceCoord.ry() = ((sourceId + 1) << 1) * MATH_D;
    }
    // for odd sources - invert the sign
    sourceCoord.ry() *= sourceId & 1 ? -1 : 1;
    return sourceCoord;
}

QPointF BaseChartItem::getSourcePositionX(size_t sourceId, bool parity) const {
    return QPointF(getSourcePositionY(sourceId, parity).y(), 0.);
}

QPointF BaseChartItem::getSourcePositionOnCircle(size_t sourceId,
                                                 size_t sourceCount) const {
    return QPointF(MATH_D * std::cos(2 * M_PI * sourceId / sourceCount),
                   MATH_D * std::sin(2 * M_PI * sourceId / sourceCount));
}

QPointF BaseChartItem::getSourcePositionInCircle() const {
    QPointF srcCoord(randomDouble(MATH_L), randomDouble(MATH_L));
    int count = 1e+4;
    while ((pow(srcCoord.x(), 2) + pow(srcCoord.y(), 2) >= pow(MATH_L, 2))
           && --count >= 0) {
        srcCoord.rx() = randomDouble(MATH_L);
        srcCoord.ry() = randomDouble(MATH_L);
    }

    return srcCoord;
}

void BaseChartItem::clearCache() {
    if (_3dDataArray) {
        delete _3dDataArray;
    }
    _3dDataArray = nullptr;

    if (_2dDataArray) {
        delete _3dDataArray;
    }
    _2dDataArray = nullptr;

    _max2d = 0;
    _max3d = 0;
}

void BaseChartItem::update() {
    if (isSourceChanged()) {
        clearCache();
    }
}

void BaseChartItem::setSourcePositionMode(
    SourcePositionMode sourcePositionMode) {
    if (_sourcePositionMode != sourcePositionMode) {
        clearCache();
        _sourcePositionMode = sourcePositionMode;
    }
}

SourcePositionMode BaseChartItem::getSourcePositionMode() {
    return _sourcePositionMode;
}

int BaseChartItem::getId() {
    return _id;
}

void BaseChartItem::setLocationSizes(double xMin,
                                     double xMax,
                                     double yMin,
                                     double yMax) {
    // clear cache if it is necessary
    if ((std::abs(_minX - xMin) > 1e-5) || (std::abs(_maxX - xMax) > 1e-5)
        || (std::abs(_minY - yMin) > 1e-5) || (std::abs(_maxY - yMax) > 1e-5)) {
        _minX = xMin;
        _maxX = xMax;
        _minY = yMin;
        _maxY = yMax;

        clearCache();
    }
}

double BaseChartItem::getXMin() const {
    return _minX;
}

double BaseChartItem::getXMax() const {
    return _maxX;
}

double BaseChartItem::getYMin() const {
    return _minY;
}

double BaseChartItem::getYMax() const {
    return _maxX;
}

double BaseChartItem::getMax2dValue() const {
    return _max2d;
}

double BaseChartItem::getMax3dValue() const {
    return _max3d;
}
