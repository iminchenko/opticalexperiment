#include "diffractiogratingchartitem.h"
#include <omp.h>

DiffractioGratingChartItem::DiffractioGratingChartItem(
        int id, DiffractionGrating * diffractionGrating
    )
{
    _id = id;
    _3dDataArray = NULL;
    _2dDataArray = NULL;

    qsrand(QTime::currentTime().second());
    _source = diffractionGrating;

    _stepX = std::abs(_maxX - _minX) / 100;
    _stepY = std::abs(_maxY - _minY) / 100;
}

struct projectionPoint {
    projectionPoint() {}

    projectionPoint(double x, std::complex<double> E_x, std::complex<double> E_y, double z) {
        this->x = x;
        this->E_x = E_x;
        this->E_y = E_y;
        this->z = z;
    }

    double x, z;
    std::complex<double> E_x, E_y;
};

QSurfaceDataArray* DiffractioGratingChartItem::fill3DSeries() {
    auto waves = _source->getWave();
    if (waves.size() == 0)
        return getDefault3dChart();

    double N1 = 10 * _source->getA() / MATH_LAMBDA;
    double N2 = 10 * _source->getW() / MATH_LAMBDA;

    //discretization steps
    _stepX = std::abs(_maxX - _minX) / 100;
    _stepY = std::abs(_maxY - _minY) / 100;

    _gradeStepX = _source->getB()/2;
    _gradeStepY = _source->getB()/2;

    size_t rowLength = static_cast<size_t> ((_maxX - _minX)/_stepX);
    size_t rowsNum = static_cast<size_t> ((_maxY - _minY)/_stepY);

    size_t pointsNum = rowsNum * rowLength;
    projectionPoint* series = new projectionPoint[pointsNum];

    QPointF currentPoint = QPointF(_minX, _minY);
    for (size_t i = 0; i < rowsNum; i++) {
        currentPoint.rx() = _minX;
        for (size_t j = 0; j < rowLength; j++) {
            series[i * rowLength + j].x = currentPoint.x();
            series[i * rowLength + j].z = currentPoint.y();
            series[i * rowLength + j].E_x = 0;
            series[i * rowLength + j].E_y = 0;

            currentPoint.rx() += _stepX;
        }
        currentPoint.ry() += _stepY;
    }

    std::vector<QPointF> sources = getSourcesPosition(waves.size());
    currentPoint.rx() = _minX;
    currentPoint.ry() = _minY;

    // half of covered by lines areas width
    double linesArea = (_source->getN3() * (_source->getA() + _source->getB()) - _source->getA()) / 2;
    // left border of lines
    double firstLineBorder = ((_maxY -_minY) / 2);
    // right border of lines
    double lastLineBorder = firstLineBorder;

    // it's our left and right border in cycle
    firstLineBorder -= linesArea;
    lastLineBorder += linesArea;

    // border of current line of grade
    double currentBorder = firstLineBorder + _source->getB();

    // for progress bar
    int lastProgress = 0;

    // calculation of values lines-by-lines of diffraction grade
    for(currentPoint.ry() = firstLineBorder; currentPoint.y() <= lastLineBorder; currentPoint.ry() += _gradeStepY) {
        if( (lastLineBorder - firstLineBorder)/(currentPoint.y() - firstLineBorder) > lastProgress) {
            lastProgress = static_cast<int>(
                        (lastLineBorder - firstLineBorder) / (currentPoint.y() - firstLineBorder)
            );
            qDebug() << lastProgress;
        }

        // moving to the next line
        if(currentPoint.y() > currentBorder) {
            currentBorder += _source->getA();
            currentPoint.ry() = currentBorder;
            currentBorder += _source->getB();
            if (currentPoint.y() > lastLineBorder) {
                // if we leave lines area
                break;
            }
        }

        for(currentPoint.rx() = _minX; currentPoint.x() <= _maxX; currentPoint.rx() += _gradeStepX) {
            // x-projection of tensity:
            std::complex<double> E_x = std::complex<double>(0,0);
            // y-projection of tensity:
            std::complex<double> E_y = std::complex<double>(0,0);
            for(size_t i = 0; i < waves.size(); i++) {
                // l_i - distance from source to grade:
                double l_i = sqrt(
                        pow(currentPoint.x() - sources[i].x(), 2) +
                        pow(currentPoint.y() - sources[i].y(), 2) +
                        pow(MATH_L, 2)
                    );
                // degree of exponent - distance to source * k'
                std::complex<double> exp_power = std::complex<double>(
                     0,
                     MATH_K * l_i
                );
                std::complex<double> coeff = std::exp(exp_power);

                E_x += waves[i].ex() * coeff;
                E_y += waves[i].ey() * coeff;
            }
            // calculating the tensity projection on shield of this point
            for (int i = 0; i < pointsNum; i++) {
                // l_i - distance from point of grade to shield:
                double l_i = sqrt(
                            pow(currentPoint.x() - series[i].x, 2) +
                            pow(currentPoint.y() - series[i].z, 2) +
                            pow(MATH_L, 2)
                        );
                std::complex<double> exp_power = std::complex<double>(
                     0,
                     MATH_K * l_i
                );
                series[i].E_x += E_x / (N1 * N2 * _source->getN3() * l_i) * std::exp(exp_power);
                series[i].E_y += E_y / (N1 * N2 * _source->getN3() * l_i) * std::exp(exp_power);
            }
        }
    }

    // forming result from projections
    _max3d = 0;
    QSurfaceDataArray *resultSeries = new QSurfaceDataArray();

    for(size_t i = 0; i < rowsNum; i++) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(static_cast<int>(rowLength));
        for(size_t j = 0; j < rowLength; j++) {
            // result coordinates of point
            double I = (norm(series[i*rowLength + j].E_x) + norm(series[i*rowLength + j].E_y)) * MATH_ALPHA * 1000;
            (*newRow)[j].setPosition(
                     QVector3D(
                         static_cast<float>(series[i*rowLength + j].x),
                         static_cast<float>(I),
                         static_cast<float>(series[i*rowLength + j].z)
                     )
            );
            // calculating max intensivity
            _max3d = I > _max3d ? I : _max3d;
        }
        resultSeries->push_back(newRow);
    }

    delete [] series;
    return resultSeries;
}

QLineSeries* DiffractioGratingChartItem::fill2DSeries(){
   // ToDo - algo?
    QLineSeries *dataArray = new QLineSeries();

    _max2d = _source->getValue(_min2dX).real();

    for (double i = _min2dX; i <= _max2dX; i += step2d) {
        double value = _source->getValue(i).real();
        dataArray->append(i, value);
        _max2d = std::max(value, _max2d);
    }

    return dataArray;
}
