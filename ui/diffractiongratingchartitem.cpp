#include "diffractiongratingchartitem.h"

struct ProjectionPoint {
    ProjectionPoint() : x(0), z(0), E_x(0), E_y(0) {}

    ProjectionPoint(double x,
                    std::complex<double> E_x,
                    std::complex<double> E_y,
                    double z)
     : x(x), z(z), E_x(E_x), E_y(E_y){}

    double x, z;
    std::complex<double> E_x, E_y;
};

DiffractionGratingChartItem::DiffractionGratingChartItem(
    int id,
    DiffractionGrating* diffractionGrating) {
    _id          = id;
    _3dDataArray = nullptr;
    _2dDataArray = nullptr;

    qsrand(static_cast<size_t>(QTime::currentTime().second()));
    _source = diffractionGrating;

    _stepX = std::abs(_maxX - _minX) / SHIELD_STEPS_X;
    _stepY = std::abs(_maxY - _minY) / SHIELD_STEPS_Y;
}

bool DiffractionGratingChartItem::isSourceChanged() {
    return _source->changed();
}

QSurfaceDataArray* DiffractionGratingChartItem::fill3DSeries() {
    auto waves = _source->getWave();
    if (waves.empty()) {
        return getDefault3dChart();
    }
    double N1 = 10 * _source->getA() / MATH_LAMBDA;
    double N2 = 10 * _source->getW() / MATH_LAMBDA;

    // discretization steps
    _stepX = std::abs(_maxX - _minX) / SHIELD_STEPS_X;
    _stepY = std::abs(_maxY - _minY) / SHIELD_STEPS_Y;

    _gradeStepX = std::abs(_maxX - _minX) / GRADE_STEPS_X;
    //_gradeStepY = _source->getB()/N1;
    _gradeStepY = _source->getB() / GRADE_STEPS_Y;

    auto rowLength = static_cast<size_t>((_maxX - _minX) / _stepX);
    auto rowsNum   = static_cast<size_t>((_maxY - _minY) / _stepY);

    size_t pointsNum = rowsNum * rowLength;
    ProjectionPoint series[pointsNum];

    QPointF currentPoint = QPointF(_minX, _minY);
    for (size_t i = 0; i < rowsNum; i++) {
        currentPoint.rx() = _minX;
        for (size_t j = 0; j < rowLength; j++) {
            series[i * rowLength + j].x   = currentPoint.x();
            series[i * rowLength + j].z   = currentPoint.y();
            series[i * rowLength + j].E_x = 0;
            series[i * rowLength + j].E_y = 0;

            currentPoint.rx() += _stepX;
        }
        currentPoint.ry() += _stepY;
    }

    std::vector<QPointF> sources = getSourcesPosition(waves.size());
    currentPoint.rx()            = _minX;
    currentPoint.ry()            = _minY;

    // it's our left and right border in cycle
    // left border of lines
    double firstLineBorder
        = -(_source->getN3() * (_source->getA() + _source->getB())
            - _source->getA())
          / 2;
    // right border of lines
    double lastLineBorder = -firstLineBorder;

    // border of current line of grade
    double currentBorder = firstLineBorder + _source->getB();

    // for progress bar
    int steps = 0;

    // calculation of values lines-by-lines of diffraction grade
    for (currentPoint.ry() = firstLineBorder;
         currentPoint.y() <= lastLineBorder;
         currentPoint.ry() += _gradeStepY) {
        // moving to the next line
        if (currentPoint.y() > currentBorder) {
            currentBorder += _source->getA();
            currentPoint.ry() = currentBorder;
            currentBorder += _source->getB();
            if (currentPoint.y() > lastLineBorder) {
                // if we leave lines area
                break;
            }
        }
        for (currentPoint.rx() = _minX; currentPoint.x() <= _maxX;
             currentPoint.rx() += _gradeStepX) {
            steps++;
            if (steps % 100 == 0) {
                qDebug() << "step: " << steps;
            }
            // x-projection of tensity:
            std::complex<double> E_x = std::complex<double>(0, 0);
            // y-projection of tensity:
            std::complex<double> E_y = std::complex<double>(0, 0);
            for (size_t i = 0; i < waves.size(); i++) {
                // l_i - distance from source to grade:
                double l_i = sqrt(pow(currentPoint.x() - sources[i].x(), 2)
                                  + pow(currentPoint.y() - sources[i].y(), 2)
                                  + pow(MATH_L, 2));
                // degree of exponent - distance to source * k'
                std::complex<double> exp_power
                    = std::complex<double>(0, MATH_K * l_i);
                std::complex<double> coeff = std::exp(exp_power);

                E_x += waves[i].ex() * coeff;
                E_y += waves[i].ey() * coeff;
            }
            // calculating the tensity projection on shield of this point
            for (size_t i = 0; i < pointsNum; i++) {
                // l_i - distance from point of grade to shield:
                double l_i = sqrt(pow(currentPoint.x() - series[i].x, 2)
                                  + pow(currentPoint.y() - series[i].z, 2)
                                  + pow(MATH_L, 2));
                std::complex<double> exp_power
                    = std::complex<double>(0, MATH_K * l_i);
                series[i].E_x += E_x / (N1 * N2 * _source->getN3() * l_i)
                                 * std::exp(exp_power);
                series[i].E_y += E_y / (N1 * N2 * _source->getN3() * l_i)
                                 * std::exp(exp_power);
            }
        }
    }
    qDebug() << "-----------------\n";
    qDebug() << "final steps: " << steps;

    // forming result from projections
    _max3d            = 0;
    auto resultSeries = new QSurfaceDataArray();

    for (size_t i = 0; i < rowsNum; i++) {
        auto newRow = new QSurfaceDataRow(static_cast<int>(rowLength));
        for (size_t j = 0; j < rowLength; j++) {
            // result coordinates of point
            double I = (norm(series[i * rowLength + j].E_x)
                        + norm(series[i * rowLength + j].E_y))
                       * MATH_ALPHA * 1000;
            (*newRow)[static_cast<int>(j)].setPosition(
                QVector3D(static_cast<float>(series[i * rowLength + j].x),
                          static_cast<float>(I),
                          static_cast<float>(series[i * rowLength + j].z)));
            // calculating max intensivity
            _max3d = I > _max3d ? I : _max3d;
        }
        resultSeries->push_back(newRow);
    }

    return resultSeries;
}

QLineSeries* DiffractionGratingChartItem::fill2DSeries() {
    // ToDo - algo?
    auto dataArray = new QLineSeries();

    _max2d = _source->getValue(_min2dX).real();

    for (double x = _min2dX; x <= _max2dX; x += _step2d) {
        double value = _source->getValue(x).real();
        dataArray->append(x, value);
        _max2d = std::max(value, _max2d);
    }

    return dataArray;
}
