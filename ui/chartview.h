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

constexpr double maxX = 1e-4, minX = -1e-4, stepX = std::abs(maxX - minX) / discritezationsStep;
constexpr double maxY = 1e-4, minY = -1e-4, stepY = std::abs(maxY - minY) / discritezationsStep;
constexpr int stepsX = (int) (maxX - minX)/stepX;
constexpr int stepsY = (int) (maxY - minY)/stepY;
constexpr double SCALE = 5e-8;

constexpr double MATH_EPSILON_0 = 8.85419e-12;
constexpr double MATH_C = 3e8;
constexpr double MATH_LAMBDA = 550e-9;
constexpr double MATH_L = 0.5;
constexpr double MATH_K = 2 * M_PI / MATH_LAMBDA;
constexpr double MATH_K_1 = M_PI / (MATH_LAMBDA * MATH_L);
constexpr double MATH_D = 1e-3;
constexpr double MATH_ALPHA = ( MATH_EPSILON_0 * MATH_C) / (8 * M_PI);

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
     QPointF getSourcePosition(size_t sourceId, bool parity);

    QSurfaceDataArray* fill3DSeries(const std::function<std::vector<Wave>()> &func);
    double fill2DSeries(
            QXYSeries *series,
            double min,
            double max,
            double step,
            const std::function<double(double)> &func
    );

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
