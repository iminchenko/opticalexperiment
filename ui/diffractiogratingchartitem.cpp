#include "diffractiogratingchartitem.h"

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

    double
        N1 = 10 * _source->getA() / MATH_LAMBDA,
        N2 = 10 * _source->getW() / MATH_LAMBDA;

    _stepX = std::abs(_maxX - _minX) / 100;
    _stepY = std::abs(_maxY - _minY) / 100;

    _gradeStepY = _source->getB()/10;
    _gradeStepX = _source->getB()/10;

    std::vector<std::vector<projectionPoint>> *series = new std::vector<std::vector<projectionPoint>>();
    QPointF currentPoint = QPointF(_minX, _minY);
    for(currentPoint.ry() = _minX; currentPoint.y() <= _maxY; currentPoint.ry() += _stepY) {
         std::vector<projectionPoint> newRow = std::vector<projectionPoint>();
         for(currentPoint.rx() = _minX; currentPoint.x() <= _maxX; currentPoint.rx() += _stepX) {
             newRow.push_back(
                  projectionPoint(
                     currentPoint.x(),
                     0, 0,
                     currentPoint.y()
                  )
             );
         }
         series->push_back(newRow);
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

    // calculation of values lines-by-lines of diffraction grade
    for(currentPoint.ry() = firstLineBorder; currentPoint.y() <= lastLineBorder; currentPoint.ry() += _gradeStepY) {
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
                std::complex<double>  coeff = std::exp(exp_power);

                E_x += waves[i].ex() * coeff;
                E_y += waves[i].ey() * coeff;
            }
            // calculating the tensity projection on shield of this point
            for(auto iter = series->begin(); iter != series->end(); iter++) {
                for(auto iter2 = (*iter).begin(); iter2 != (*iter).end(); iter2++) {
                    // l_i - distance from point of grade to shield:
                    double l_i = sqrt(
                                pow(currentPoint.x() - (*iter2).x, 2) +
                                pow(currentPoint.y() - (*iter2).z, 2) +
                                pow(MATH_L, 2)
                            );
                    std::complex<double> exp_power = std::complex<double>(
                         0,
                         MATH_K * l_i
                    );
                    (*iter2).E_x += E_x / (N1 * N2 * _source->getN3() * l_i) * std::exp(exp_power) * 100000.;
                    (*iter2).E_y += E_y / (N1 * N2 * _source->getN3() * l_i) * std::exp(exp_power) * 100000.;
                }
            }
        }
    }

    // forming result from projections
    _max3d = 0;
    QSurfaceDataArray *resultSeries = new QSurfaceDataArray();
    for(auto iter = series->begin(); iter != series->end(); iter++) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow();
        for(auto iter2 = (*iter).begin(); iter2 != (*iter).end(); iter2++) {
            // result coordinates of point
            double I = (norm((*iter2).E_x) + norm((*iter2).E_x))* MATH_ALPHA * 10000000;
            newRow->push_back(
                 QSurfaceDataItem(
                     QVector3D(
                         (float) (*iter2).x,
                         (float) I,
                         (float) (*iter2).z
                     )
                 )
            );
            // calculating max intensivity
            _max3d = I > _max3d ? I : _max3d;
        }
        resultSeries->push_back(newRow);
    }
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
