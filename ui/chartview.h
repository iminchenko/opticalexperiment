#ifndef CONSTRUCTORCHART_H
#define CONSTRUCTORCHART_H

#include <QtCharts>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QScrollArea>
#include "deviceconfigs/wave.h"
#include "ui/parametersmanager.h"

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

enum AlgorithmType {
    first = 0,
    second
};

class ChartView : public QObject
{
    Q_OBJECT
public:
    ChartView(int id, QTabWidget *_tabWidget);
    ~ChartView();

    void update(double min, double max,
                           double step, const std::function<double(double)> &func);

    void update3d(const std::function<std::vector<Wave>()> &func);
    int getId();
    int getTabIndex();
    void updateTabIndexAfterRemovingTab(int idx);

public slots:
    void updateScaleFactor();
    void updateValues();
    void changeAlgorithm(int);
private:
    void update3d();
    void initChart2D(QLayout *layout);
    void initChart3D(QLayout *layout);

    QSurfaceDataArray* getDefaultChart();
    QPointF getSourcePosition(size_t sourceId, size_t sourceCount) const;
    QPointF getSourcePositionY(size_t sourceId, bool parity) const;
    QPointF getSourcePositionX(size_t sourceId, bool parity) const;
    QPointF getSourcePositionOnCircle(size_t sourceId, size_t sourceCount) const;
    QPointF getSourcePositionInCircle(size_t sourceId, size_t) const;
    
    SourcePositionMode getSourcePositionMode() const;

    QSurfaceDataArray* fill3DSeriesFirstAlgo();
    QSurfaceDataArray* fill3DSeriesSecondAlgo();

    double fill2DSeries(
            QXYSeries *series,
            double min,
            double max,
            double step,
            const std::function<double(double)> &func
    );

    int _id;
    double _max;
    std::vector<Wave> _waves;
    QScrollArea *_scrollArea;
    int _tabIdx;
    AlgorithmType _algoType = AlgorithmType::first;

    double _maxX = 1e-4, _minX = -1e-4, _stepX = std::abs(_maxX - _minX) / discritezationsStep;
    double _maxY = 1e-4, _minY = -1e-4, _stepY = std::abs(_maxY - _minY) / discritezationsStep;
    int _stepsX = (int) (_maxX - _minX)/_stepX;
    int _stepsY = (int) (_maxY - _minY)/_stepY;

    QTabWidget *_tabWidget;
    QTab *_tab;
    QLayout *_layout;
    QWidget *_container;
    QLineEdit *_xMinEditField;
    QLineEdit *_xMaxEditField;
    QLineEdit *_yMinEditField;
    QLineEdit *_yMaxEditField;
    QSlider *_slider;
    QLabel *_maxLabel;
    QComboBox *_algorithmSelector;

    QChart *_chart;
    QChartView *_chartView;

    Q3DSurface *_surfaceW;
    QWidget *_surfaceContainer;
    Q3DTheme *_theme;
    QSurfaceDataProxy *_3dProxyFunc;
    QSurface3DSeries *_3dSeriesFunc;
};

#endif // CONSTRUCTORCHART_H
