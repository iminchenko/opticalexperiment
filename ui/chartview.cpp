#include "chartview.h"

#include <QSurfaceDataItem>

ChartView::ChartView(int id, QLayout *layout)
    : _id(id), _layout(layout)
{
    QLayout *containerLayout = new QVBoxLayout();
    initChart3D(containerLayout);
    initChart2D(containerLayout);

    _container = new QWidget();
    _container->setLayout(containerLayout);

    _layout->addWidget(_container);
}

void ChartView::initChart2D(QLayout *layout) {
    _chart = new QChart();
    _chart->setTitle("Shield chart #" + QString::number(_id));

    _chart->addSeries(new QLineSeries(_chart));

    _chart->createDefaultAxes();
    _chart->axisX()->setRange(xMinus, xPlus);
    _chart->axisY()->setRange(0, 0.01);
    _chart->legend()->hide();
    _chart->setAnimationOptions(QChart::SeriesAnimations);

    _chartView = new QChartView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing, true);
    _chartView->setMinimumHeight(CHART_MINIMUM_HEIGHT);

    layout->addWidget(_chartView);
}

void ChartView::initChart3D(QLayout *layout) {
    _surfaceW = new Q3DSurface(Q_NULLPTR, Q_NULLPTR);

    _3dProxyFunc = new QSurfaceDataProxy();
    _3dSeriesFunc = new QSurface3DSeries(_3dProxyFunc);
    _3dSeriesFunc->setDrawMode(QSurface3DSeries::DrawSurface);
    _surfaceW->addSeries(_3dSeriesFunc);

    _surfaceW->setTitle("Intensivity on shield #" + QString::number(_id));

    // Axis styling
    _surfaceW->axisX()->setLabelFormat("%.2f");
    _surfaceW->axisZ()->setLabelFormat("%.2f");

    _surfaceW->axisX()->setRange(minX,maxX);
    _surfaceW->axisY()->setRange(0, 1);
    _surfaceW->axisZ()->setRange(minY, maxY);

    _surfaceW->axisX()->setLabelAutoRotation(90);
    _surfaceW->axisY()->setLabelAutoRotation(90);
    _surfaceW->axisZ()->setLabelAutoRotation(90);

    _surfaceW->axisX()->setTitle(QStringLiteral("x"));
    _surfaceW->axisZ()->setTitle(QStringLiteral("y"));
    _surfaceW->axisY()->setTitle(QStringLiteral("I"));

    _surfaceW->axisX()->setTitleVisible(true);
    _surfaceW->axisY()->setTitleVisible(true);
    _surfaceW->axisZ()->setTitleVisible(true);

    _surfaceW->axisX()->setTitleFixed(false);
    _surfaceW->axisY()->setTitleFixed(false);
    _surfaceW->axisZ()->setTitleFixed(false);

    _surfaceW->setShadowQuality(Q3DSurface::ShadowQualityNone);

    _surfaceW->setReflection(false);

    _theme = new Q3DTheme();
    _theme->setType(Q3DTheme::ThemePrimaryColors);
    _theme->setAmbientLightStrength(0.8f);
    _theme->setLightStrength(0.8f);
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.3, Qt::blue);
    gr.setColorAt(0.6, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    _surfaceW->seriesList().at(0)->setBaseGradient(gr);
    _surfaceW->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);


    _theme->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    _surfaceW->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    //init data
    QSurfaceDataArray *dataArray = getDefaultChart();
    _3dProxyFunc->resetArray(dataArray);
    _surfaceW->show();

    _surfaceContainer = QWidget::createWindowContainer(_surfaceW);
    _surfaceContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    _surfaceContainer->setMinimumHeight(600);
    _surfaceW->setOrthoProjection(true);
    _surfaceW->setFlipHorizontalGrid(true);
    layout->addWidget(_surfaceContainer);
}


ChartView::~ChartView() {
    _layout->removeWidget(_container);
//    delete _chartView;
//    delete _surfaceContainer;
}

QSurfaceDataArray* ChartView::getDefaultChart() {
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    QSurfaceDataRow *newRow = new QSurfaceDataRow();
    newRow->push_back(QSurfaceDataItem(QVector3D( minX, 0 , minY)));
    newRow->push_back(QSurfaceDataItem(QVector3D( maxX, 0 , minY)));
    *dataArray << newRow;

    newRow = new QSurfaceDataRow();
    newRow->push_back(QSurfaceDataItem(QVector3D( minX, 0 , maxY)));
    newRow->push_back(QSurfaceDataItem(QVector3D( maxX, 0 , maxY)));
    *dataArray << newRow;
    return dataArray;
}

void ChartView::update(double min, double max,
                       double step, const std::function<double(double)> &func) {
    double maxValue = fillSeries(dynamic_cast<QXYSeries*>(_chart->series()[0]), min, max, step, func);
    _chart->axisY()->setRange(0, maxValue * 1.2);
}

void ChartView::update3d(const std::function<std::vector<Wave>()> &func) {
    QSurfaceDataArray *newArray = fill3DSeries(func);
    _3dProxyFunc->resetArray(newArray);
}

QPointF ChartView::getSourcePosition(int sourceId, bool parity) {
    if (!parity) {
        // even number of sources
        if (sourceId&1) {
            return QPointF(0, (- (0.5 + (sourceId << 1))) *  MATH_D);
        }
        return QPointF(0, (0.5 + (sourceId << 1)) *  MATH_D);
    }
    // odd number of sources
    if (sourceId&1) {
        return QPointF(0, (((sourceId + 1) << 1) >> 1) *  MATH_D);
    }
    return QPointF(0, -(((sourceId + 1) << 1) >> 1) *  MATH_D);

}

QSurfaceDataArray* ChartView::fill3DSeries(const std::function<std::vector<Wave>()> &func) {
    std::vector<Wave> waves = func();
    if (waves.size() == 0)
        return getDefaultChart();
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    qDebug() << "alpha:" << MATH_ALPHA;
    qDebug() << "K':" << MATH_K_1;
    qDebug() << "K:" << MATH_K;
    bool parity = !(waves.size()&1);
    bool error = false;
    double max = 0;
    if (func) {
        dataArray->reserve(stepsY);
        QPointF currentPoint = QPointF(minY, minX);
        for (; currentPoint.y() <= maxY; currentPoint.ry() += stepY) {
            // y-line
            QSurfaceDataRow *newRow = new QSurfaceDataRow();
            currentPoint.rx() = minX;
            for(; currentPoint.x() <= maxX; currentPoint.rx() += stepX) {
                // x-line
                std::complex<double>
                    // x-projection's multiplies of intencivity:
                    sum_x_1 = std::complex<double>(0,0),
                    sum_x_2 = std::complex<double>(0,0),
                    // y-projection's multiplies of intencivity:
                    sum_y_1 = std::complex<double>(0,0),
                    sum_y_2 = std::complex<double>(0,0);

                for(size_t i = 0; i < waves.size(); i++) {
                    // calculation summ for each source
                    QPointF currentPointCopy = currentPoint;
                    std::complex<double> exp_power = std::complex<double>(
                         0,
                         MATH_K_1 * pow((currentPoint - getSourcePosition(i, parity)).manhattanLength(),2)
                    );

                    sum_x_1 += waves[i].ex() * std::exp(exp_power);
                    sum_x_2 += std::conj(waves[i].ex()) * std::exp(-exp_power);

                    sum_y_1 += waves[i].ey() * std::exp(exp_power);
                    sum_y_2 += std::conj(waves[i].ey()) * std::exp(-exp_power);
                }
                // Result intencivity projections:
                if ((sum_x_1 * sum_x_2).imag() != 0) {
                    error = true;
                }
                double I_x = abs((sum_x_1 * std::conj(sum_x_1)).real());
                double I_y = abs((sum_y_1 * std::conj(sum_y_1)).real());
                //result
                newRow->push_back(
                     QSurfaceDataItem(
                         QVector3D(
                             currentPoint.x(),
                             (I_x + I_y)* MATH_ALPHA * SCALE,
                             currentPoint.y()
                         )
                     )
                );
                if (((I_x + I_y)* MATH_ALPHA * SCALE) > max) {
                   max = (I_x + I_y)* MATH_ALPHA * SCALE;
                }
            }

            *dataArray << newRow;
        }
    }
    if (error)
        qDebug() << "error!";
    qDebug() << "I max:" << max;
    return dataArray;
}

double ChartView::fillSeries(QXYSeries *series, double min, double max,
    double step, const std::function<double(double)> &func) {

    double maxValue = 1e-7;
    if (!series)
        return maxValue;

    series->clear();

    for (double i = min; i <= max; i += step) {
        double value = func(i);
        series->append(i, value);
        maxValue = std::max(value, maxValue);
    }

    return maxValue;
}

int ChartView::getId() {
    return _id;
}

