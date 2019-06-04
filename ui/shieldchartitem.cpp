#include "shieldchartitem.h"

ShieldChartItem::ShieldChartItem(int id, Display* source) {
    _id = id;
    _source = source;

    qsrand(static_cast<size_t>(QTime::currentTime().second()));
}

bool ShieldChartItem::isSourceChanged() {
    return _source->changed();
}

QSurfaceDataArray* ShieldChartItem::fill3DSeries() {
    auto waves = _source->getWave();
    if (waves.empty()) {
        return getDefault3dChart();
    }
    _stepX = std::abs(_maxX - _minX) / discritezationsStep;
    _stepY = std::abs(_maxY - _minY) / discritezationsStep;

    // result matrix of intencivity
    auto dataArray = new QSurfaceDataArray();
    dataArray->reserve(_stepsY);
    // max value of intencivity
    _max3d = 0;
    // Coordinates of calculated current point
    QPointF currentPoint = QPointF(_minY, _minX);
    // Sources positions array
    // Sources positions array
    std::vector<QPointF> sources = getSourcesPosition(waves.size());
    // y-row cycle
    for (; currentPoint.y() <= _maxY; currentPoint.ry() += _stepY) {
        auto newRow = new QSurfaceDataRow();
        // setting current point at the begining of calculation area
        currentPoint.rx() = _minX;
        // x-row cycle
        for (; currentPoint.x() <= _maxX; currentPoint.rx() += _stepX) {
            // x-projection of intencivity:
            std::complex<double> sum_x = std::complex<double>(0, 0);
            // y-projection of intencivity:
            std::complex<double> sum_y = std::complex<double>(0, 0);
            // calculation summ for each source
            for (size_t i = 0; i < waves.size(); i++) {
                // po - distance from source to shield:
                double po
                    = (sources[i].x() * currentPoint.x()
                       + sources[i].y() * currentPoint.y() + MATH_L * MATH_L)
                      / sqrt(pow(sources[i].x(), 2) + pow(sources[i].y(), 2)
                             + pow(MATH_L, 2));
                // degree of exponent - distance to source * k'
                std::complex<double> exp_power
                    = std::complex<double>(0, MATH_K * po);
                sum_x += waves[i].ex() * std::exp(exp_power);
                sum_y += waves[i].ey() * std::exp(exp_power);
            }
            // result intencivity's projections:
            double I_x = pow(std::abs(sum_x), 2);
            double I_y = pow(std::abs(sum_y), 2);

            // result intencivity
            double I = (I_x + I_y) * MATH_ALPHA * SCALE;

            // result coordinates of point
            newRow->push_back(QSurfaceDataItem(
                QVector3D(static_cast<float>(currentPoint.x()),
                          static_cast<float>(I),
                          static_cast<float>(currentPoint.y()))));
            // calculating max intensivity
            _max3d = I > _max3d ? I : _max3d;
        }
        // inserting x-row in matrix
        *dataArray << newRow;
    }
    return dataArray;
}

QLineSeries* ShieldChartItem::fill2DSeries() {
    auto dataArray = new QLineSeries();

    _max2d = _source->getValue(_min2dX).real();

    for (double x = _min2dX; x <= _max2dX; x += _step2d) {
        double value = _source->getValue(x).real();
        dataArray->append(x, value);
        _max2d = std::max(value, _max2d);
    }

    return dataArray;
}
