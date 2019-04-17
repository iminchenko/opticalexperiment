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
#include "ui/chartitem.h"


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
    QPointF getSourcePositionInCircle() const;

    SourcePositionMode getSourcePositionMode() const;

    QSurfaceDataArray* fill3DSeriesFirstAlgo();
    QSurfaceDataArray* fill3DSeriesSecondAlgo();

    double randomDouble(double max) const;

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

    //warning! kostil is here:)
    std::vector<QPointF> _sourcesForRand = std::vector<QPointF>();

    double _maxX = 1e-4, _minX = -1e-4;
    double _stepX = std::abs(_maxX - _minX) / discritezationsStep;
    double _maxY = 1e-4, _minY = -1e-4;
    double _stepY = std::abs(_maxY - _minY) / discritezationsStep;
    int _stepsX = static_cast<int>((_maxX - _minX)/_stepX);
    int _stepsY = static_cast<int>((_maxY - _minY)/_stepY);

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
