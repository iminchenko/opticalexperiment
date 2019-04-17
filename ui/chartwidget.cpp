#include "chartwidget.h"
#include <exception>

ChartWidget::ChartWidget(QWidget *parent)
{
    _containerLayout = new QVBoxLayout();

    //adding chart selector
    _chartSelector = new QComboBox();
    connect( _chartSelector, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeChart(int)));

    QFormLayout *chartLayout = new QFormLayout();
    chartLayout->addRow("Select chart:", _chartSelector);

    QGroupBox *chartsControls = new QGroupBox();
    chartsControls->setLayout(chartLayout);
    chartsControls->setTitle("Charts:");
    _containerLayout->addWidget(chartsControls);

    // adding source mode selector
    _sourceModeSelector = new QComboBox();
    _sourceModeSelector->addItems(QStringList() << "(x, 0)" << "(0, x)" << "On Circle" << "In Circle");
    connect( _sourceModeSelector, SIGNAL(currentIndexChanged(int)),
            this, SLOT(sourceModeChanged(int)));

    QFormLayout *sourceModeLayout = new QFormLayout();
    sourceModeLayout->addRow("Select source position mode:", _sourceModeSelector);

    QGroupBox *sourceModeControls = new QGroupBox();
    sourceModeControls->setLayout(sourceModeLayout);
    sourceModeControls->setTitle("Charts:");
    _containerLayout->addWidget(sourceModeControls);

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
    _containerLayout->addWidget(maxValueBlock);

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
    connect(updateBtn, SIGNAL(clicked()), this, SLOT(updateSizes()));

    QGroupBox *xyControls = new QGroupBox();
    xyControls->setLayout(sizeControlsLayout);
    xyControls->setTitle("X,Y controls:");
    _containerLayout->addWidget(xyControls);

    //adding charts
    initChart3D();
    initChart2D();

    //adding scroll area
    _activatedContainer = new QFrame();
    _activatedContainer->setLayout(_containerLayout);

    //_scrollArea = new QScrollArea(this);
    //_scrollArea->setWidget(_activatedContainer);

    //_tabIdx = _tabWidget->addTab(_scrollArea,"Shield chart #" + QString::number(_id));

    _deactivatedContainer = new QFrame();
    QLayout* deactivatedLayout = new QVBoxLayout();
    QLabel *deactivatedLabel = new QLabel("Для отображения результатов моделирования добавьте экран");
    deactivatedLayout->addWidget(deactivatedLabel);
    _deactivatedContainer->setLayout(deactivatedLayout);
    _deactivatedContainer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    _changableLayout = new QStackedLayout();
    _changableLayout->addWidget(_deactivatedContainer);
    _changableLayout->addWidget(_activatedContainer);

    QWidget* inner = new QFrame();
    inner->setLayout(_changableLayout);
    //inner->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setWidget(inner);
    this->setWidgetResizable(true);
}

void ChartWidget::initChart2D() {
    _chart = new QChart();
    //_chart->setTitle("Shield chart #" + QString::number(_id));

    _chart->addSeries(new QLineSeries(_chart));

    _chart->createDefaultAxes();
    _chart->axisX()->setRange(xMinus, xPlus);
    _chart->axisY()->setRange(0, 0.01);
    _chart->legend()->hide();
    _chart->setAnimationOptions(QChart::SeriesAnimations);

    _chartView = new QChartView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing, true);
    _chartView->setMinimumHeight(CHART_MINIMUM_HEIGHT);

    _containerLayout->addWidget(_chartView);
}

void ChartWidget::initChart3D() {
    _surfaceW = new Q3DSurface(Q_NULLPTR, Q_NULLPTR);

    _3dProxyFunc = new QSurfaceDataProxy();
    _3dSeriesFunc = new QSurface3DSeries(_3dProxyFunc);
    _3dSeriesFunc->setDrawMode(QSurface3DSeries::DrawSurface);
    _surfaceW->addSeries(_3dSeriesFunc);

    //_surfaceW->setTitle("Intensivity on shield #" + QString::number(_id));

    // Axis styling
    _surfaceW->axisX()->setLabelFormat("%.2f");
    _surfaceW->axisZ()->setLabelFormat("%.2f");

    _surfaceW->axisX()->setRange(getXMin(), getXMax());
    _surfaceW->axisY()->setRange(0, 1);
    _surfaceW->axisZ()->setRange(getYMin(), getYMax());

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
    //QSurfaceDataArray *dataArray = getDefaultChart();
    //_3dProxyFunc->resetArray(dataArray);

    //creating container
    _surfaceContainer = QWidget::createWindowContainer(_surfaceW);
    _surfaceContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    _surfaceContainer->setMinimumHeight(600);
    _surfaceW->setOrthoProjection(true);
    _surfaceW->setFlipHorizontalGrid(true);

    //adding container element into layout
    _containerLayout->addWidget(_surfaceContainer);
}


void ChartWidget::updateSizes(){
    BaseChartItem *chart = getActiveChart();
    if(chart == NULL)
        return;
    try {
        double
                xMin = getXMin(),
                xMax = getXMax(),
                yMin = getYMin(),
                yMax = getYMax();
        if (xMin >= xMax) {
            throw std::runtime_error("Uncorrect borders: x-min > x-max!");
        }
        if (yMin >= yMax) {
            throw std::runtime_error("Uncorrect borders: x-min > x-max!");
        }
        chart->setLocationSizes(xMin, xMax, yMin, yMax);
    }
    catch (std::runtime_error err) {
        (new QErrorMessage(this))->showMessage(err.what());
    }

    // updating 3d chart
    QSurfaceDataArray* series3d = chart->get3dSeries();
    _3dProxyFunc->resetArray(series3d);
    _surfaceW->axisX()->setRange(chart->getXMin(), chart->getXMax());
    _surfaceW->axisZ()->setRange(chart->getYMin(), chart->getYMax());

    _maxLabel->setText(QString::number(chart->getMax3dValue()));
}

void ChartWidget::sourceModeChanged(int positionMode) {
    BaseChartItem *chart = getActiveChart();
    if(chart == NULL)
        return;

    SourcePositionMode mode = static_cast<SourcePositionMode>(positionMode);

    chart->setSourcePositionMode(mode);

    // updating 3d chart
    QSurfaceDataArray* series3d = chart->get3dSeries();
    _3dProxyFunc->resetArray(series3d);
    _surfaceW->axisX()->setRange(chart->getXMin(), chart->getXMax());
    _surfaceW->axisZ()->setRange(chart->getYMin(), chart->getYMax());

    _maxLabel->setText(QString::number(chart->getMax3dValue()));
}

void ChartWidget::updateScaleFactor() {
    BaseChartItem *chart = getActiveChart();
    if(chart == NULL)
        return;
    int value = _slider->value();
     _surfaceW->axisY()->setRange(0, (chart->getMax3dValue() * 100 / value));
}


BaseChartItem* ChartWidget::getActiveChart(){
    int id = _chartSelector->currentIndex();
    if (id == -1)
        return NULL;
    return  _charts->at(id).get();
}

void ChartWidget::deactivate() {
    _changableLayout->setCurrentWidget(_deactivatedContainer);
}

void ChartWidget::activate() {
    _changableLayout->setCurrentWidget(_activatedContainer);
    if (_chartSelector->currentIndex() == -1)
        _chartSelector->setCurrentIndex(0);
        changeChart(0);
}

void ChartWidget::changeChart(int id) {
   updateChart();
}

void ChartWidget::addChart(int id){
    _chartSelector->addItem(
        "Shield #" + QString::number((_charts->at(id))->getId())
    );
    if (_charts->size() == 1) {
        // first chart added
        activate();
        _chartSelector->setCurrentIndex(0);
    }
}

void ChartWidget::removeChart(int id) {
    if (_chartSelector->currentIndex() == id) {
        _chartSelector->removeItem(id);
        if (_charts->size() == 0) {
            _chartSelector->setCurrentIndex(-1);
            deactivate();
        } else {
            _chartSelector->setCurrentIndex(0);
        }
    } else{
        _chartSelector->removeItem(id);
    }
}

void ChartWidget::updateChart(){
    int id = _chartSelector->currentIndex();
    if (id == -1) {
         deactivate();
         return;
    }

    BaseChartItem* chart = (_charts->at(id)).get();

    // updating 3d chart
    QSurfaceDataArray* series3d = chart->get3dSeries();
    _3dProxyFunc->resetArray(series3d);
    _surfaceW->axisX()->setRange(chart->getXMin(), chart->getXMax());
    _surfaceW->axisZ()->setRange(chart->getYMin(), chart->getYMax());

    _maxLabel->setText(QString::number(chart->getMax3dValue()));

    // updating 2d chart
    _chart->removeAllSeries();
    auto series2d = dynamic_cast<QXYSeries*>(chart->get2dSeries());
    _chart->addSeries(series2d);

    // updating modelling parameters
    _xMinEditField->setText(QString::number(chart->getXMin()));
    _xMaxEditField->setText(QString::number(chart->getXMax()));
    _yMinEditField->setText(QString::number(chart->getYMin()));
    _yMaxEditField->setText(QString::number(chart->getYMax()));

    _sourceModeSelector->setCurrentIndex(chart->getSourcePositionMode());
    //ToDo
    //chart.getSourcePosition();

}

void ChartWidget::setChartsArray(std::vector<std::shared_ptr<BaseChartItem>> *charts){
    _charts = charts;
}

double ChartWidget::getXMin() {
    bool ok = true;
    double res = _xMinEditField->text().toDouble(&ok);
    if (!ok) {
        throw std::runtime_error("Failed to parse x-min value");
    }
    return res;
}

double ChartWidget::getYMin() {
    bool ok = true;
    double res = _yMinEditField->text().toDouble(&ok);
    if (!ok) {
        throw std::runtime_error("Failed to parse y-min value");
    }
    return res;
}

double ChartWidget::getXMax() {
    bool ok = true;
    double res = _xMaxEditField->text().toDouble(&ok);
    if (!ok) {
        throw std::runtime_error("Failed to parse x-max value");
    }
    return res;
}

double ChartWidget::getYMax() {
    bool ok = true;
    double res = _yMaxEditField->text().toDouble(&ok);
    if (!ok) {
        throw std::runtime_error("Failed to parse y-max value");
    }
    return res;
}

