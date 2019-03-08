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

    _surfaceW->axisX()->setRange(minX, maxX);
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

    //setting of pallete of colours
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::blue);
    gr.setColorAt(0.33, QColor(Qt::green).darker(100));
    gr.setColorAt(0.66, Qt::yellow);
    gr.setColorAt(1, Qt::red);

    _surfaceW->seriesList().at(0)->setBaseGradient(gr);
    _surfaceW->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    //init graph with data
    QSurfaceDataArray *dataArray = getDefaultChart();
    _3dProxyFunc->resetArray(dataArray);

    //creating container
    _surfaceContainer = QWidget::createWindowContainer(_surfaceW);
    _surfaceContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    _surfaceContainer->setMinimumHeight(600);
    _surfaceW->setOrthoProjection(true);
    _surfaceW->setFlipHorizontalGrid(true);

    //adding container element into layout
    layout->addWidget(_surfaceContainer);
}

ChartView::~ChartView() {
    delete _surfaceContainer;
    delete _chartView;
    _layout->removeWidget(_container);

}

QSurfaceDataArray* ChartView::getDefaultChart() {
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;

    QSurfaceDataRow *newRow = new QSurfaceDataRow();
    newRow->push_back(QSurfaceDataItem(QVector3D(minX, 0 , minY)));
    newRow->push_back(QSurfaceDataItem(QVector3D(maxX, 0 , minY)));
    *dataArray << newRow;

    newRow = new QSurfaceDataRow();
    newRow->push_back(QSurfaceDataItem(QVector3D(minX, 0 , maxY)));
    newRow->push_back(QSurfaceDataItem(QVector3D(maxX, 0 , maxY)));
    *dataArray << newRow;

    return dataArray;
}

QPointF ChartView::getSourcePosition(size_t sourceId, size_t sourceCount) const
{
    SourcePositionMode mod = SourcePositionMode::OnlyX;
    // parity of sources number
    bool parity = !(sourceCount&1); 
    
    switch (mod) {
    case SourcePositionMode::OnlyX:
        return getSourcePositionX(sourceId, parity);
    case SourcePositionMode::OnlyY: 
        return getSourcePositionY(sourceId, parity);
    case SourcePositionMode::OnCircle:
        return getSourcePositionOnCircle(sourceId, sourceCount);
    default:
        return getSourcePositionX(sourceId, parity);
    }
}

void ChartView::update(double min,
                       double max,
                       double step,
                       const std::function<double(double)> &func) {
    double maxValue = fill2DSeries(dynamic_cast<QXYSeries*>(_chart->series()[0]), min, max, step, func);
    _chart->axisY()->setRange(0, maxValue * 1.2);
}

void ChartView::update3d(const std::function<std::vector<Wave>()> &func) {
    QSurfaceDataArray *newArray = fill3DSeries(func);
    _3dProxyFunc->resetArray(newArray);
}

QPointF ChartView::getSourcePositionY(size_t sourceId, bool parity) const {
    QPointF sourceCoord(0,0);
    if (!parity) {
        // sequence for even number of sources
        sourceCoord.ry() = (-(0.5 + (sourceId << 1))) * MATH_D;
    } else {
        // sequence for odd number of sources
        sourceCoord.ry() = ((sourceId + 1) << 1) * MATH_D;
    }
    //for odd sources - invert the sign
    sourceCoord.ry() *= sourceId & 1 ? -1 : 1;
    return sourceCoord;
}

QPointF ChartView::getSourcePositionX(size_t sourceId, bool parity) const
{
    QPointF sourceCoord(getSourcePositionY(sourceId, parity));
    sourceCoord.setX(sourceCoord.ry());    
    sourceCoord.setY(0.);    
    return sourceCoord;
}

QPointF ChartView::getSourcePositionOnCircle(size_t sourceId, size_t sourceCount) const
{
    return QPointF(MATH_D * std::cos(2 * M_PI * sourceId / sourceCount),
                   MATH_D * std::sin(2 * M_PI * sourceId / sourceCount));
}

QPointF ChartView::getSourcePositionInCircle(size_t sourceId, size_t) const
{
   
}

QSurfaceDataArray* ChartView::fill3DSeries(const std::function<std::vector<Wave>()> &func) {
    std::vector<Wave> waves = func();
    if (waves.size() == 0)
        return getDefaultChart();

    // result matrix of intencivity
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(stepsY);
    // max value of intencivity
    double max = 0;
    // Coordinates of calculated current point
    QPointF currentPoint = QPointF(minY, minX);

    // y-row cycle
    for (; currentPoint.y() <= maxY; currentPoint.ry() += stepY) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow();
        //setting current point at the begining of calculation area
        currentPoint.rx() = minX;
        // x-row cycle
        for(; currentPoint.x() <= maxX; currentPoint.rx() += stepX) {
            // x-projection of intencivity:
            std::complex<double> sum_x = std::complex<double>(0,0);
            // y-projection of intencivity:
            std::complex<double> sum_y = std::complex<double>(0,0);

            // calculation summ for each source
            for(size_t i = 0; i < waves.size(); i++) {
                // degree of exponent - distance to source * k'
                std::complex<double> exp_power = std::complex<double>(
                     0,
                     MATH_K_1 * pow((currentPoint - getSourcePosition(i, waves.size())).manhattanLength(), 2)
                );
                sum_x += waves[i].ex() * std::exp(exp_power);
                sum_y += waves[i].ey() * std::exp(exp_power);
            }
            // result intencivity's projections:
            double I_x = std::abs((sum_x * std::conj(sum_x)).real());
            double I_y = std::abs((sum_y * std::conj(sum_y)).real());

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
            max = I > max ? I : max;
        }
        // inserting x-row in matrix
        *dataArray << newRow;
    }
    // ToDo: resizing of chart by maximum value
    qDebug() << "I max:" << max;

    return dataArray;
}

double ChartView::fill2DSeries(QXYSeries *series, double min, double max,
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

