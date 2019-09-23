#pragma once

#include "deviceconfigs/display.h"
#include "deviceconfigs/wave.h"
#include <QScrollArea>
#include <QtCharts>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>

constexpr double X_MINUS                = -50;
constexpr double X_PLUS                 = 50;
constexpr short int SIZE_DISCRETIZATION = 500;
constexpr int DISCRETIZATION_STEP      = 1e+3;
constexpr int CHART_MINIMUM_HEIGHT     = 175;

constexpr double X_DEFAULT_MIN = -1e-4;
constexpr double X_DEFAULT_MAX = 1e-4;
constexpr double Y_DEFAULT_MIN = -1e-4;
constexpr double Y_DEFAULT_MAX = 1e-4;

constexpr size_t GRADE_STEPS_X  = 100;
constexpr size_t GRADE_STEPS_Y  = 20;
constexpr size_t SHIELD_STEPS_X = 100;
constexpr size_t SHIELD_STEPS_Y = 100;

constexpr double SCALE = 5e-8;

constexpr double MATH_EPSILON_0 = 8.85419e-12;
constexpr double MATH_C         = 3e8;
constexpr double MATH_LAMBDA    = 550e-9;
constexpr double MATH_L         = 0.5;
constexpr double MATH_K         = 2 * M_PI / MATH_LAMBDA;
constexpr double MATH_K_1       = M_PI / (MATH_LAMBDA * MATH_L);
constexpr double MATH_D         = 1e-3;
constexpr double MATH_ALPHA     = (MATH_EPSILON_0 * MATH_C) / (8 * M_PI);

using namespace QtDataVisualization;

enum SourcePositionMode { OnlyX = 0, OnlyY, OnCircle, InCircle };

class BaseChartItem {
public:
    virtual ~BaseChartItem();
    int getId();

    void setLocationSizes(double xMin, double xMax, double yMin, double yMax);
    void setSourcePositionMode(SourcePositionMode sourcePositionMode);
    SourcePositionMode getSourcePositionMode();

    double getXMin() const;
    double getXMax() const;
    double getYMin() const;
    double getYMax() const;

    void update();

    double getMax2dValue() const;
    double getMax3dValue() const;

    QSurfaceDataArray* getDefault3dChart();
    QXYSeries* getDefault2dChart();

    QSurfaceDataArray* get3dSeries();
    QXYSeries* get2dSeries();

protected:
    void clearCache();

    std::vector<QPointF> getSourcesPosition(size_t sourcesNum);

    QPointF getSourcePosition(SourcePositionMode mod,
                              size_t sourceId,
                              size_t sourceCount) const;
    QPointF getSourcePositionY(size_t sourceId, bool parity) const;
    QPointF getSourcePositionX(size_t sourceId, bool parity) const;
    QPointF getSourcePositionOnCircle(size_t sourceId,
                                      size_t sourceCount) const;
    QPointF getSourcePositionInCircle() const;

    double randomDouble(double max) const;

    // for overloading
    virtual QSurfaceDataArray* fill3DSeries() = 0;
    virtual QLineSeries* fill2DSeries()       = 0;
    virtual bool isSourceChanged()            = 0;

    // modeling  settings:
    double _maxX  = X_DEFAULT_MAX;
    double _minX  = X_DEFAULT_MIN;
    double _maxY  = Y_DEFAULT_MAX;
    double _minY  = Y_DEFAULT_MIN;
    double _stepX = std::abs(_maxX - _minX) / DISCRETIZATION_STEP;
    double _stepY = std::abs(_maxY - _minY) / DISCRETIZATION_STEP;
    int _stepsX   = static_cast<int>((_maxX - _minX) / _stepX);
    int _stepsY   = static_cast<int>((_maxY - _minY) / _stepY);

    double _min2dX = X_MINUS;
    double _max2dX = X_PLUS;
    double _step2d = X_PLUS / SIZE_DISCRETIZATION;

    // results of modelling
    QSurfaceDataArray* _3dDataArray = nullptr;
    QLineSeries* _2dDataArray = nullptr;

    // maximum values of series for scalling
    double _max3d = 0;
    double _max2d = 0;

    // sources for random arrangement
    std::vector<QPointF> _sourcesForRand = std::vector<QPointF>();

    // id of shield
    int _id;

    SourcePositionMode _sourcePositionMode = SourcePositionMode::OnlyX;
};
