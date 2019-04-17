#ifndef BASECHARTITEM_H
#define BASECHARTITEM_H

#include <QtCharts>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QScrollArea>
#include "deviceconfigs/wave.h"
#include "deviceconfigs/display.h"

constexpr double xMinus = -50;
constexpr double xPlus = 50;
constexpr short int sizeDiscretization = 500;
constexpr int discritezationsStep = 1e+3;
constexpr int CHART_MINIMUM_HEIGHT = 175;

constexpr double xDefaultMin = -1e-4;
constexpr double xDefaultMax = 1e-4;
constexpr double yDefaultMin = -1e-4;
constexpr double yDefaultMax = 1e-4;

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

enum SourcePositionMode {
    OnlyX = 0,
    OnlyY,
    OnCircle,
    InCircle
};

//ToDo: additional tools list
class BaseChartItem
{
public:
    virtual ~BaseChartItem();
    int getId();

    void setLocationSizes(double xMin, double xMax, double yMin, double yMax);
    void setSourcePositionMode(SourcePositionMode sourcePositionMode);
    SourcePositionMode getSourcePositionMode();

    double getXMin();
    double getXMax();
    double getYMin();
    double getYMax();

    void update();

    double getMax2dValue();
    double getMax3dValue();

    QSurfaceDataArray* getDefault3dChart();
    QXYSeries* getDefault2dChart();

    QSurfaceDataArray *get3dSeries();
    QXYSeries *get2dSeries();

protected:
    std::vector<QPointF> getSourcesPosition(int sourcesNum);

    QPointF getSourcePosition(SourcePositionMode mod, size_t sourceId, size_t sourceCount) const;
    QPointF getSourcePositionY(size_t sourceId, bool parity) const;
    QPointF getSourcePositionX(size_t sourceId, bool parity) const;
    QPointF getSourcePositionOnCircle(size_t sourceId, size_t sourceCount) const;
    QPointF getSourcePositionInCircle() const;

    double randomDouble(double max) const;

    // for overwriting
    virtual QSurfaceDataArray* fill3DSeries() = 0;
    virtual QLineSeries* fill2DSeries() = 0;

    // modelation settings:
    double _maxX = 1e-4, _minX = -1e-4;
    double _maxY = 1e-4, _minY = -1e-4;
    double _stepX = std::abs(_maxX - _minX) / discritezationsStep;
    double _stepY = std::abs(_maxY - _minY) / discritezationsStep;
    int _stepsX = static_cast<int>((_maxX - _minX)/_stepX);
    int _stepsY = static_cast<int>((_maxY - _minY)/_stepY);

    double _min2dX = xMinus, _max2dX = xPlus, step2d = xPlus / sizeDiscretization;

    //source of data for generations
    // must be added by classes

    //results of modelling
    QSurfaceDataArray *_3dDataArray;
    QLineSeries *_2dDataArray;

    // maximum values of series for scalling
    double _max3d = 0, _max2d = 0;

    //sources for random arrangement
    std::vector<QPointF> _sourcesForRand = std::vector<QPointF>();

    // id of shield
    int _id;

    SourcePositionMode _sourcePositionMode;

};

#endif // BASECHARTITEM_H