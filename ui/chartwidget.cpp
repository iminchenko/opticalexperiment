#include "chartwidget.h"
#include <exception>

ChartWidget::ChartWidget(QWidget *parent) : QScrollArea(parent) {
    auto containerLayout = new QVBoxLayout();

    // adding chart selector
    _chartSelector = new QComboBox();
    connect(_chartSelector,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateChart()));

    auto *chartLayout = new QFormLayout();
    chartLayout->addRow("Select chart:", _chartSelector);

    auto *chartsControls = new QGroupBox();
    chartsControls->setLayout(chartLayout);
    chartsControls->setTitle("Charts:");
    containerLayout->addWidget(chartsControls);

    // adding source mode selector
    _sourceModeSelector = new QComboBox();
    _sourceModeSelector->addItems(QStringList() << "(x, 0)"
                                                << "(0, x)"
                                                << "On Circle"
                                                << "In Circle");
    connect(_sourceModeSelector,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(sourceModeChanged(int)));

    auto *sourceModeLayout = new QFormLayout();
    sourceModeLayout->addRow("Select source position mode:",
                             _sourceModeSelector);

    auto *sourceModeControls = new QGroupBox();
    sourceModeControls->setLayout(sourceModeLayout);
    sourceModeControls->setTitle("Charts:");
    containerLayout->addWidget(sourceModeControls);

    // adding scaling controlls
    auto *scalingLayout = new QFormLayout();

    _slider = new QSlider(Qt::Horizontal);
    _slider->setMaximum(100);
    _slider->setMinimum(10);
    connect(static_cast<QSlider *>(_slider),
            SIGNAL(sliderReleased()),
            this,
            SLOT(updateScaleFactor()));

    _maxLabel = new QLabel("0");

    scalingLayout->addRow("Scale factor:", _slider);
    scalingLayout->addRow("Max value:", _maxLabel);

    auto *maxValueBlock = new QGroupBox();
    maxValueBlock->setLayout(scalingLayout);
    maxValueBlock->setTitle("Scaling chart:");
    containerLayout->addWidget(maxValueBlock);

    // adding xy - controls
    _xMinEditField = new QLineEdit();
    _xMaxEditField = new QLineEdit();
    _yMinEditField = new QLineEdit();
    _yMaxEditField = new QLineEdit();
    _xMinEditField->setText(QString::number(X_DEFAULT_MIN));
    _xMaxEditField->setText(QString::number(X_DEFAULT_MAX));
    _yMinEditField->setText(QString::number(Y_DEFAULT_MIN));
    _yMaxEditField->setText(QString::number(Y_DEFAULT_MAX));

    auto *sizeControlsLayout = new QFormLayout();
    sizeControlsLayout->addRow("xMin", _xMinEditField);
    sizeControlsLayout->addRow("xMax", _xMaxEditField);
    sizeControlsLayout->addRow("yMin", _yMinEditField);
    sizeControlsLayout->addRow("yMax", _yMaxEditField);

    auto *updateBtn = new QPushButton("Update");
    sizeControlsLayout->addWidget(updateBtn);
    connect(updateBtn, SIGNAL(clicked()), this, SLOT(updateSizes()));

    auto *xyControls = new QGroupBox();
    xyControls->setLayout(sizeControlsLayout);
    xyControls->setTitle("X,Y controls:");
    containerLayout->addWidget(xyControls);

    // adding charts
    initChart3D(containerLayout);
    initChart2D(containerLayout);

    // adding scroll area
    _activatedContainer = new QFrame();
    _activatedContainer->setLayout(containerLayout);

    //_scrollArea = new QScrollArea(this);
    //_scrollArea->setWidget(_activatedContainer);

    //_tabIdx = _tabWidget->addTab(_scrollArea,"Shield chart #" +
    // QString::number(_id));

    _deactivatedContainer      = new QFrame();
    QLayout *deactivatedLayout = new QVBoxLayout();
    QLabel *deactivatedLabel   = new QLabel(
        "Для отображения результатов моделирования добавьте экран");
    deactivatedLabel->setWordWrap(true);

    deactivatedLayout->addWidget(deactivatedLabel);
    _deactivatedContainer->setLayout(deactivatedLayout);
    _deactivatedContainer->setSizePolicy(
        QSizePolicy::Preferred,
        //                QSizePolicy::MinimumExpanding,
        QSizePolicy::MinimumExpanding);

    _changableLayout = new QStackedLayout();
    _changableLayout->addWidget(_deactivatedContainer);
    _changableLayout->addWidget(_activatedContainer);

    auto *inner = new QFrame();
    inner->setLayout(_changableLayout);
    //    inner->setSizePolicy(
    //        QSizePolicy::MinimumExpanding,
    //        QSizePolicy::MinimumExpanding
    //    );
    this->setWidget(inner);
    this->setWidgetResizable(true);
}

void ChartWidget::initChart2D(QLayout *containerLayout) {
    _chart = new QChart();
    //_chart->setTitle("Shield chart #" + QString::number(_id));

    _chart->addSeries(new QLineSeries(_chart));

    _chart->createDefaultAxes();
    _chart->axes(Qt::Horizontal).back()->setRange(xMinus, xPlus);
    _chart->axes(Qt::Vertical).back()->setRange(0, 0.01);
    _chart->legend()->hide();
    _chart->setAnimationOptions(QChart::SeriesAnimations);

    auto chartView = new QChartView(_chart);
    chartView->setRenderHint(QPainter::Antialiasing, true);
    chartView->setMinimumHeight(CHART_MINIMUM_HEIGHT);

    containerLayout->addWidget(chartView);
}

void ChartWidget::initChart3D(QLayout *containerLayout) {
    _surfaceW = new Q3DSurface(Q_NULLPTR, Q_NULLPTR);

    _3dProxyFunc      = new QSurfaceDataProxy();
    auto Series3dFunc = new QSurface3DSeries(_3dProxyFunc);
    Series3dFunc->setDrawMode(QSurface3DSeries::DrawSurface);
    // Series3dFunc->setFlatShadingEnabled(true);
    _surfaceW->addSeries(Series3dFunc);

    //_surfaceW->setTitle("Intensivity on shield #" + QString::number(_id));

    // Axis styling
    _surfaceW->axisX()->setLabelFormat("%.2f");
    _surfaceW->axisZ()->setLabelFormat("%.2f");

    _surfaceW->axisX()->setRange(static_cast<float>(getXMin()),
                                 static_cast<float>(getXMax()));
    _surfaceW->axisY()->setRange(0, 1);
    _surfaceW->axisZ()->setRange(static_cast<float>(getYMin()),
                                 static_cast<float>(getYMax()));

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

    // setting of pallete of colours
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::blue);
    gr.setColorAt(0.33, QColor(Qt::green).darker(100));
    gr.setColorAt(0.66, Qt::yellow);
    gr.setColorAt(1, Qt::red);

    _surfaceW->seriesList().at(0)->setBaseGradient(gr);
    _surfaceW->seriesList().at(0)->setColorStyle(
        Q3DTheme::ColorStyleRangeGradient);

    // creating container
    auto surfaceContainer = QWidget::createWindowContainer(_surfaceW);
    surfaceContainer->setSizePolicy(QSizePolicy::Minimum,
                                    QSizePolicy::Expanding);
    surfaceContainer->setMinimumHeight(600);
    _surfaceW->setOrthoProjection(true);
    _surfaceW->setFlipHorizontalGrid(true);

    // adding container element into layout
    containerLayout->addWidget(surfaceContainer);
}

void ChartWidget::updateSizes() {
    BaseChartItem *chart = getActiveChart();
    if (chart == nullptr) {
        return;
    }
    try {
        double xMin = getXMin();
        double xMax = getXMax();
        double yMin = getYMin();
        double yMax = getYMax();

        if (xMin >= xMax) {
            throw std::runtime_error("Uncorrect borders: x-min > x-max!");
        }
        if (yMin >= yMax) {
            throw std::runtime_error("Uncorrect borders: x-min > x-max!");
        }
        chart->setLocationSizes(xMin, xMax, yMin, yMax);
    } catch (std::runtime_error err) {
        (new QErrorMessage(this))->showMessage(err.what());
    }

    // updating 3d chart
    update3dChart();
}

void ChartWidget::sourceModeChanged(int positionMode) {
    BaseChartItem *chart = getActiveChart();
    if (chart == nullptr) {
        return;
    }
    auto mode = static_cast<SourcePositionMode>(positionMode);

    chart->setSourcePositionMode(mode);

    // updating 3d chart
    update3dChart();
}

void ChartWidget::updateScaleFactor() {
    BaseChartItem *chart = getActiveChart();
    if (chart == nullptr) {
        return;
    }
    int value = _slider->value();
    _surfaceW->axisY()->setRange(
        0, static_cast<float>(chart->getMax3dValue() * 100 / value));
}

BaseChartItem *ChartWidget::getActiveChart() {
    auto id = static_cast<int>(_chartSelector->currentIndex());
    if (id == -1) {
        return nullptr;
    }
    return _charts->at(static_cast<size_t>(id)).get();
}

void ChartWidget::deactivate() {
    _changableLayout->setCurrentWidget(_deactivatedContainer);
}

void ChartWidget::activate() {
    _changableLayout->setCurrentWidget(_activatedContainer);
    if (_chartSelector->currentIndex() == -1) {
        _chartSelector->setCurrentIndex(0);
        updateChart();
    }
}

void ChartWidget::addChart(int id) {
    _chartSelector->addItem(
        "Shield #"
        + QString::number((_charts->at(static_cast<size_t>(id)))->getId()));
    if (_charts->size() == 1) {
        // first chart added
        activate();
        _chartSelector->setCurrentIndex(0);
    }
}

void ChartWidget::removeChart(int id) {
    if (_chartSelector->currentIndex() == id) {
        _chartSelector->removeItem(id);
        if (_charts->empty()) {
            _chartSelector->setCurrentIndex(-1);
            deactivate();
        } else {
            _chartSelector->setCurrentIndex(0);
        }
    } else {
        _chartSelector->removeItem(id);
    }
}

void ChartWidget::updateChart() {
    int id = _chartSelector->currentIndex();
    if (id == -1) {
        deactivate();
        return;
    }
    BaseChartItem *chart = (_charts->at(static_cast<size_t>(id))).get();

    // updating charts
    update3dChart(chart);
    update2dChart(chart);

    // updating modelling area size settings
    updateAreaSizeSettings(chart);
    // updating sources localization's mode
    _sourceModeSelector->setCurrentIndex(chart->getSourcePositionMode());
    qDebug() << chart->getSourcePositionMode();
}

void ChartWidget::update3dChart() {
    BaseChartItem *chart = getActiveChart();
    if (chart == nullptr) {
        return;
    }
    // updating chart
    update3dChart(chart);
}

void ChartWidget::update2dChart() {
    BaseChartItem *chart = getActiveChart();
    if (chart == nullptr) {
        return;
    }
    // updating chart
    update2dChart(chart);
}

void ChartWidget::update3dChart(BaseChartItem *chart) {
    // updating 3d chart
    QSurfaceDataArray *series3d = chart->get3dSeries();
    _3dProxyFunc->resetArray(series3d);

    _surfaceW->axisX()->setRange(static_cast<float>(chart->getXMin()),
                                 static_cast<float>(chart->getXMax()));
    _surfaceW->axisZ()->setRange(static_cast<float>(chart->getYMin()),
                                 static_cast<float>(chart->getYMax()));

    updateScaleFactor();
    _maxLabel->setText(QString::number(chart->getMax3dValue()));
}

void ChartWidget::update2dChart(BaseChartItem *chart) {
    // updating 2d chart
    _chart->removeAllSeries();
    auto series2d = dynamic_cast<QXYSeries *>(chart->get2dSeries());
    _chart->addSeries(series2d);
}

void ChartWidget::updateAreaSizeSettings(BaseChartItem *chart) {
    // updating modelling parameters
    _xMinEditField->setText(QString::number(chart->getXMin()));
    _xMaxEditField->setText(QString::number(chart->getXMax()));
    _yMinEditField->setText(QString::number(chart->getYMin()));
    _yMaxEditField->setText(QString::number(chart->getYMax()));
}

void ChartWidget::setChartsArray(
    std::vector<std::shared_ptr<BaseChartItem>> *charts) {
    _charts = charts;
}

double ChartWidget::getXMin() const {
    bool ok    = true;
    double res = _xMinEditField->text().toDouble(&ok);
    if (!ok) {
        throw std::runtime_error("Failed to parse x-min value");
    }
    return res;
}

double ChartWidget::getYMin() const {
    bool ok    = true;
    double res = _yMinEditField->text().toDouble(&ok);
    if (!ok) {
        throw std::runtime_error("Failed to parse y-min value");
    }
    return res;
}

double ChartWidget::getXMax() const {
    bool ok    = true;
    double res = _xMaxEditField->text().toDouble(&ok);
    if (!ok) {
        throw std::runtime_error("Failed to parse x-max value");
    }
    return res;
}

double ChartWidget::getYMax() const {
    bool ok    = true;
    double res = _yMaxEditField->text().toDouble(&ok);
    if (!ok) {
        throw std::runtime_error("Failed to parse y-max value");
    }
    return res;
}
