#ifndef CONSTRUCTORCHART_H
#define CONSTRUCTORCHART_H

#include <QtCharts>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include "deviceconfigs/wave.h"

constexpr double xMinus = -50;
constexpr double xPlus = 50;
constexpr short int sizeDiscretization = 500;
constexpr int discritezationsStep = 1000;
constexpr int CHART_MINIMUM_HEIGHT = 175;

const double maxX = 0.0001, minX =  -0.0001, stepX = abs(maxX - minX) / discritezationsStep,
       maxY = 0.0001, minY = -0.0001, stepY = abs(maxY - minY) / discritezationsStep;
const int stepsX = (int)(maxX - minX)/stepX,
    stepsY = (int)(maxY - minY)/stepY;
constexpr double SCALE = 0.00000005;

const double MATH_EPSILON_0 = 8.85419*pow(10,-12);
const double MATH_C = 3 * pow(10,8);
const double MATH_LAMBDA = 550 * pow(10,-9);
const double MATH_L = 0.5;
const double MATH_K = 2 * M_PI / MATH_LAMBDA;
const double MATH_K_1 = M_PI / (MATH_LAMBDA * MATH_L);
const double MATH_D = pow(10,-3);
const double MATH_ALPHA = ( MATH_EPSILON_0 * MATH_C) / (8 * M_PI);

using namespace QtDataVisualization;

class ChartView
{
public:
    ChartView(int id, QLayout *layout);
    ~ChartView();

    void update(double min, double max,
                           double step, const std::function<double(double)> &func);

    void update3d(const std::function<std::vector<Wave>()> &func);
    int getId();

private:
    void initChart2D(QLayout *layout);
    void initChart3D(QLayout *layout);

    QSurfaceDataArray* getDefaultChart();
    QSurfaceDataArray* fill3DSeries(const std::function<std::vector<Wave>()> &func);
    double fillSeries(QXYSeries *series, double min, double max,
                double step, const std::function<double(double)> &func);

    QPointF getSourcePosition(int sourceId, bool parity);

    int _id;
    QLayout *_layout;
    QWidget *_container;

    QChart *_chart;
    QChartView *_chartView;

    Q3DSurface *_surfaceW;
    QWidget *_surfaceContainer;
    Q3DTheme *_theme;
    QSurfaceDataProxy *_3dProxyFunc;
    QSurface3DSeries *_3dSeriesFunc;
};

#endif // CONSTRUCTORCHART_H
