#ifndef CONSTRUCTORCHART_H
#define CONSTRUCTORCHART_H

#include <QtCharts>

constexpr double xMinus = -50;
constexpr double xPlus = 50;
constexpr short int sizeDiscretization = 500;

constexpr int CHART_MINIMUM_HEIGHT = 175;

class ChartView
{
public:
    ChartView(int id, QLayout *layout);
    ~ChartView();

    void update(double min, double max,
                           double step, const std::function<double(double)> &func);

    int getId();

private:
    double fillSeries(QXYSeries *series, double min, double max,
                double step, const std::function<double(double)> &func);

    int _id;
    QLayout *_layout;
    QChart *_chart;
    QChartView *_chartView;
};

#endif // CONSTRUCTORCHART_H
