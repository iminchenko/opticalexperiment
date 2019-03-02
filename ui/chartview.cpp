#include "chartview.h"


ChartView::ChartView(int id, QLayout *layout)
    : _id(id), _layout(layout)
{
    _chart = new QChart();
    _chart->setTitle("Shield chart #" + QString::number(id));

    _chart->addSeries(new QLineSeries(_chart));

    _chart->createDefaultAxes();
    _chart->axisX()->setRange(xMinus, xPlus);
    _chart->axisY()->setRange(0, 0.01);
    _chart->legend()->hide();
    _chart->setAnimationOptions(QChart::SeriesAnimations);

    _chartView = new QChartView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing, true);
    _chartView->setMinimumHeight(CHART_MINIMUM_HEIGHT);

    _layout->addWidget(_chartView);

}

ChartView::~ChartView() {
    _layout->removeWidget(_chartView);
    delete _chartView;
}

void ChartView::update(double min,
                       double max,
                       double step,
                       const std::function<double(double)> &func) {
    double maxValue = fillSeries(dynamic_cast<QXYSeries*>(_chart->series()[0]),
                                 min,
                                 max,
                                 step,
                                 func);

    _chart->axisY()->setRange(0, maxValue * 1.2);
}

double ChartView::fillSeries(QXYSeries *series,
                             double min,
                             double max,
                             double step,
                             const std::function<double(double)> &func) {

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

