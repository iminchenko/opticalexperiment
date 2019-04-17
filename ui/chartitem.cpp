#include "chartitem.h"

ChartItem::ChartItem(int id, Display* source)

{
    _id = id;
    _3dDataArray = NULL;
    _2dDataArray = NULL;

    qsrand(QTime::currentTime().second());

    _source = source;
}

QSurfaceDataArray* ChartItem::fill3DSeries() {
    auto waves = _source->getWave();
    if (waves.size() == 0)
        return getDefault3dChart();

    // result matrix of intencivity
    QSurfaceDataArray *dataArray = new QSurfaceDataArray();
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
        QSurfaceDataRow *newRow = new QSurfaceDataRow();
        //setting current point at the begining of calculation area
        currentPoint.rx() = _minX;
        // x-row cycle
        for(; currentPoint.x() <= _maxX; currentPoint.rx() += _stepX) {
            // x-projection of intencivity:
            std::complex<double> sum_x = std::complex<double>(0,0);
            // y-projection of intencivity:
            std::complex<double> sum_y = std::complex<double>(0,0);
            // calculation summ for each source
            for(size_t i = 0; i < waves.size(); i++) {
                // po - distance from source to shield:
                double po =
                    (
                        sources[i].x() * currentPoint.x() +
                        sources[i].y() * currentPoint.y() +
                        MATH_L * MATH_L
                    ) / sqrt(
                        pow(sources[i].x(), 2) +
                        pow(sources[i].y(), 2) +
                        pow(MATH_L, 2)
                    );
                // degree of exponent - distance to source * k'
                std::complex<double> exp_power = std::complex<double>(
                     0,
                     MATH_K * po
                );
                sum_x += waves[i].ex() * std::exp(exp_power);
                sum_y += waves[i].ey() * std::exp(exp_power);

            }
            // result intencivity's projections:
            double I_x = pow(std::abs(sum_x), 2);
            double I_y = pow(std::abs(sum_y), 2);

            // result intencivity
            double I = (I_x + I_y)* MATH_ALPHA * SCALE;

            // result coordinates of point
            newRow->push_back(
                 QSurfaceDataItem(
                     QVector3D(
                         (float) currentPoint.x(),
                         (float) I,
                         (float) currentPoint.y()
                     )
                 )
            );
            // calculating max intensivity
            _max3d = I > _max3d ? I : _max3d;
        }
        // inserting x-row in matrix
        *dataArray << newRow;
    }
    return dataArray;
}

QLineSeries* ChartItem::fill2DSeries() {
    QLineSeries *dataArray = new QLineSeries();

    _max2d = _source->getValue(_min2dX).real();

    for (double i = _min2dX; i <= _max2dX; i += step2d) {
        double value = _source->getValue(i).real();
        dataArray->append(i, value);
        _max2d = std::max(value, _max2d);
    }

    return dataArray;
}
