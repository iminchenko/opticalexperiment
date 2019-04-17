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

    double _maxX = 1e-4, _minX = -1e-4;
    double _maxY = 1e-4, _minY = -1e-4;
    double _stepX = std::abs(_maxX - _minX) / 10;
    double _stepY = std::abs(_maxY - _minY) / 10;
    int _stepsX = static_cast<int>((_maxX - _minX)/_stepX);
    int _stepsY = static_cast<int>((_maxY - _minY)/_stepY);
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

    std::vector<std::vector<projectionPoint>> *series = new std::vector<std::vector<projectionPoint>>();
    QPointF currentPoint = QPointF(_minX, _minY);
    for(; currentPoint.y() <= _maxY; currentPoint.ry() += _stepY) {
         std::vector<projectionPoint> newRow = std::vector<projectionPoint>();
         currentPoint.rx() = _minX;
         for(; currentPoint.x() <= _maxX; currentPoint.rx() += _stepX) {
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

    // border of current line of grade
    double currentBorder = _minX;

    // calculation of values lines-by-lines of diffraction grade
    for(currentPoint.ry() = _minY; currentPoint.y() <= _maxY; currentPoint.ry() += _stepY) {
        // moving to the next line
        if(currentPoint.y() >= currentBorder) {
            currentPoint.ry() = currentBorder + _source->getB();
            currentBorder += _source->getB() + _source->getA();
            if (currentPoint.y() > _maxY) {
                break;
            }
        }

        for(currentPoint.rx() = _minX; currentPoint.x() <= _maxX; currentPoint.rx() += _stepX*10) {
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
                E_x += waves[i].ex() * std::exp(exp_power);
                E_y += waves[i].ey() * std::exp(exp_power);
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
                    (*iter2).E_x += E_x / (N1 * N2 * _source->getN3() * l_i) * std::exp(exp_power);
                    (*iter2).E_y += E_y / (N1 * N2 * _source->getN3() * l_i) * std::exp(exp_power);
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
            double I = (norm((*iter2).E_x) + norm((*iter2).E_x))* MATH_ALPHA * SCALE;
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
