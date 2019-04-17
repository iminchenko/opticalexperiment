#include "chartview.h"
#include <QTabBar>

#include <QSurfaceDataItem>
#include <QPushButton>
#include  "math.h"
ChartView::ChartView(int id, QTabWidget *tabWidget)
    : _id(id),
      _tabWidget(tabWidget) {
    QLayout *containerLayout = new QVBoxLayout();

    //adding algorithm type controlls
    _algorithmSelector = new QComboBox();
    _algorithmSelector->addItems(QStringList() << "First algorithm" << "Second algorithm");
    connect( _algorithmSelector, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeAlgorithm(int)));

    QFormLayout *algoLayout = new QFormLayout();
    algoLayout->addRow("Algorithm type:", _algorithmSelector);

    QGroupBox *algorithmControls = new QGroupBox();
    algorithmControls->setLayout(algoLayout);
    algorithmControls->setTitle("Algorithm settings:");
    containerLayout->addWidget(algorithmControls);

    //adding scaling controlls
    QFormLayout *scalingLayout = new QFormLayout();

    _slider = new QSlider(Qt::Horizontal);
    _slider->setMaximum(100);
    _slider->setMinimum(10);
    connect((QSlider*) _slider, SIGNAL(sliderReleased()), this, SLOT(updateScaleFactor()));

    _maxLabel = new QLabel("0");

    scalingLayout->addRow("Scale factor:", _slider);
    scalingLayout->addRow("Max value:",_maxLabel);

    QGroupBox *maxValueBlock = new QGroupBox();
    maxValueBlock->setLayout(scalingLayout);
    maxValueBlock->setTitle("Scaling chart:");
    containerLayout->addWidget(maxValueBlock);

    //adding xy - controls
    _xMinEditField = new QLineEdit();
    _xMaxEditField = new QLineEdit();
    _yMinEditField = new QLineEdit();
    _yMaxEditField = new QLineEdit();
    _xMinEditField->setText(QString::number(xDefaultMin));
    _xMaxEditField->setText(QString::number(xDefaultMax));
    _yMinEditField->setText(QString::number(yDefaultMin));
    _yMaxEditField->setText(QString::number(yDefaultMax));

    QFormLayout *sizeControlsLayout = new QFormLayout();
    sizeControlsLayout->addRow("xMin", _xMinEditField);
    sizeControlsLayout->addRow("xMax", _xMaxEditField);
    sizeControlsLayout->addRow("yMin", _yMinEditField);
    sizeControlsLayout->addRow("yMax", _yMaxEditField);

    QPushButton *updateBtn = new QPushButton("Update");
    sizeControlsLayout->addWidget(updateBtn);
    connect(updateBtn, SIGNAL(clicked()), this, SLOT(updateValues()));

    QGroupBox *xyControls = new QGroupBox();
    xyControls->setLayout(sizeControlsLayout);
    xyControls->setTitle("X,Y controls:");
    containerLayout->addWidget(xyControls);

    //adding charts
    initChart3D(containerLayout);
    initChart2D(containerLayout);

    //adding scroll area
    QWidget* inner = new QFrame();
    inner->setLayout(containerLayout);

    _scrollArea = new QScrollArea();
    _scrollArea->setWidgetResizable(true);
    _scrollArea->setWidget(inner);

    _tabIdx = _tabWidget->addTab(_scrollArea,"Shield chart #" + QString::number(_id));

    //  Для функции randomDouble
    qsrand(QTime::currentTime().second());
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

    _surfaceW->axisX()->setRange(_minX, _maxX);
    _surfaceW->axisY()->setRange(0, 1);
    _surfaceW->axisZ()->setRange(_minY, _maxY);

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
    _tabWidget->removeTab(_tabIdx);
    delete _scrollArea;
}

void ChartView::updateScaleFactor(){
    int value = _slider->value();
     _surfaceW->axisY()->setRange(0, (_max * 100 / value));
}

void ChartView::updateValues(){
    update3d();
}

void ChartView::changeAlgorithm(int type) {
    _algoType = static_cast<AlgorithmType>(type);
    update3d();
}

void ChartView::updateTabIndexAfterRemovingTab(int idx) {
    if (idx < _tabIdx) _tabIdx--;
}

QSurfaceDataArray* ChartView::getDefaultChart() {
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;

    QSurfaceDataRow *newRow = new QSurfaceDataRow();
    newRow->push_back(QSurfaceDataItem(QVector3D(_minX, 0, _minY)));
    newRow->push_back(QSurfaceDataItem(QVector3D(_maxX, 0, _minY)));
    *dataArray << newRow;

    newRow = new QSurfaceDataRow();
    newRow->push_back(QSurfaceDataItem(QVector3D(_minX, 0, _maxY)));
    newRow->push_back(QSurfaceDataItem(QVector3D(_maxX, 0, _maxY)));
    *dataArray << newRow;

    return dataArray;
}

SourcePositionMode ChartView::getSourcePositionMode() const {
    return SourcePositionMode::OnlyX;//PARAM_MANAGER.getSourcePositionMode();
}

QPointF ChartView::getSourcePosition(size_t sourceId, size_t sourceCount) const {
    SourcePositionMode mod = getSourcePositionMode();
    // parity of sources number
    bool parity = !(sourceCount&1);

    switch (mod) {
    case SourcePositionMode::OnlyX:
        return getSourcePositionX(sourceId, parity);
    case SourcePositionMode::OnlyY:
        return getSourcePositionY(sourceId, parity);
    case SourcePositionMode::OnCircle:
        return getSourcePositionOnCircle(sourceId, sourceCount);
    case SourcePositionMode::InCircle:
        return getSourcePositionInCircle();
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
    _waves = func();
    update3d();
}

void ChartView::update3d() {
    _maxX = _xMaxEditField->text().toDouble();
    _minX = _xMinEditField->text().toDouble();
    _maxY = _yMaxEditField ->text().toDouble();
    _minY = _yMinEditField->text().toDouble();

    _stepX = std::abs(_maxX - _minX) / discritezationsStep;
    _stepY = std::abs(_maxY - _minY) / discritezationsStep;

    _stepsX = (int) (_maxX - _minX)/_stepX;
    _stepsY = (int) (_maxY - _minY)/_stepY;

    QSurfaceDataArray *newArray;
    switch(_algoType) {
        case AlgorithmType::first :
            newArray = fill3DSeriesFirstAlgo();
            break;
        case AlgorithmType::second :
            newArray = fill3DSeriesSecondAlgo();
            break;
        default:
            qDebug() << "unknown AlgotithmType";
            newArray = getDefaultChart();
            break;
    }


    _surfaceW->axisX()->setRange(_minX, _maxX);
    _surfaceW->axisY()->setRange(0, 1);
    _surfaceW->axisZ()->setRange(_minY, _maxY);
    updateScaleFactor();

    _3dProxyFunc->resetArray(newArray);

    _maxLabel->setText(QString::number(_max));
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

QPointF ChartView::getSourcePositionX(size_t sourceId, bool parity) const {
    QPointF sourceCoord(getSourcePositionY(sourceId, parity));
    sourceCoord.setX(sourceCoord.ry());
    sourceCoord.setY(0.);
    return sourceCoord;
}

QPointF ChartView::getSourcePositionOnCircle(size_t sourceId,
                                             size_t sourceCount) const {
    return QPointF(MATH_D * std::cos(2 * M_PI * sourceId / sourceCount),
                   MATH_D * std::sin(2 * M_PI * sourceId / sourceCount));
}

QPointF ChartView::getSourcePositionInCircle() const {
    QPointF srcCoord(randomDouble(MATH_L), randomDouble(MATH_L));
    int count = 1e+4;
    while ((pow(srcCoord.x(), 2) + pow(srcCoord.y(), 2) >= pow(MATH_L, 2)) &&
           --count >= 0) {
        srcCoord.rx() = randomDouble(MATH_L);
        srcCoord.ry() = randomDouble(MATH_L);
    }

    return srcCoord;
}

double ChartView::randomDouble(double max) const {
    return (double)(qrand()) / RAND_MAX * max;
}

QSurfaceDataArray* ChartView::fill3DSeriesFirstAlgo() {
    if (_waves.size() == 0)
        return getDefaultChart();

    // result matrix of intencivity
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(_stepsY);
    // max value of intencivity
    _max = 0;
    // Coordinates of calculated current point
    QPointF currentPoint = QPointF(_minY, _minX);
    // Sources positions array
    std::vector<QPointF> sources = std::vector<QPointF>();
    if(getSourcePositionMode() == SourcePositionMode::InCircle) {
        if (_sourcesForRand.size() < _waves.size()) {
            for(size_t i = _sourcesForRand.size(); i < _waves.size(); i++) {
                _sourcesForRand.push_back(getSourcePosition(i, _waves.size()));
            }
        }
        for(size_t i = 0; i < _waves.size(); i++) {
            sources.push_back(_sourcesForRand[i]);
        }
    } else {
        for(size_t i = 0; i < _waves.size(); i++) {
            sources.push_back(getSourcePosition(i, _waves.size()));
        }
    }
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
            for(size_t i = 0; i < _waves.size(); i++) {
                // degree of exponent - distance to source * k'
                std::complex<double> exp_power = std::complex<double>(
                     0,
                     MATH_K_1 * pow((currentPoint - sources[i]).manhattanLength(), 2)
                );
                sum_x += _waves[i].ex() * std::exp(exp_power);
                sum_y += _waves[i].ey() * std::exp(exp_power);
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
            _max = I > _max ? I : _max;
        }
        // inserting x-row in matrix
        *dataArray << newRow;
    }

    return dataArray;
}

QSurfaceDataArray* ChartView::fill3DSeriesSecondAlgo() {
    if (_waves.size() == 0)
        return getDefaultChart();

    // result matrix of intencivity
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(_stepsY);
    // max value of intencivity
    _max = 0;
    // Coordinates of calculated current point
    QPointF currentPoint = QPointF(_minY, _minX);
    // Sources positions array
    // Sources positions array
    std::vector<QPointF> sources = std::vector<QPointF>();
    if(getSourcePositionMode() == SourcePositionMode::InCircle) {
        if (_sourcesForRand.size() < _waves.size()) {
            for(size_t i = _sourcesForRand.size(); i < _waves.size(); i++) {
                _sourcesForRand.push_back(getSourcePosition(i, _waves.size()));
            }
        }
        for(size_t i = 0; i < _waves.size(); i++) {
            sources.push_back(_sourcesForRand[i]);
        }
    } else {
        for(size_t i = 0; i < _waves.size(); i++) {
            sources.push_back(getSourcePosition(i, _waves.size()));
        }
    }
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
            for(size_t i = 0; i < _waves.size(); i++) {
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
                sum_x += _waves[i].ex() * std::exp(exp_power);
                sum_y += _waves[i].ey() * std::exp(exp_power);

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
            _max = I > _max ? I : _max;
        }
        // inserting x-row in matrix
        *dataArray << newRow;
    }

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

int ChartView::getTabIndex() {
    return _tabIdx;
}
