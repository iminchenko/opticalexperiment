#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QtCharts>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include "ui/basechartitem.h"

using namespace QtDataVisualization;

class ChartWidget : public QScrollArea
{
    Q_OBJECT
public:
    ChartWidget(QWidget *parent = 0);

    // for chartHandler
    void addChart(int id);
    void removeChart(int id);
    void updateChart();
    void setChartsArray(std::vector<std::shared_ptr<BaseChartItem>>* charts);

public slots:
    //for combobox
    void changeChart(int id);
    void sourceModeChanged(int sourceMode);
    // for setttings
    void updateSizes();
    void updateScaleFactor();

private:
    void initChart3D();
    void initChart2D();

    QSurfaceDataArray* getDefaultChart();

    double getXMin();
    double getXMax();
    double getYMin();
    double getYMax();

    void deactivate();
    void activate();

    BaseChartItem* getActiveChart();

    QComboBox *_chartSelector, *_sourceModeSelector;
    QScrollArea *_scrollArea;
    QLayout *_containerLayout, *_deactivatedLayout;
    QWidget *_container;
    QLineEdit *_xMinEditField;
    QLineEdit *_xMaxEditField;
    QLineEdit *_yMinEditField;
    QLineEdit *_yMaxEditField;
    QSlider *_slider;
    QLabel *_maxLabel;

    QChart *_chart;
    QChartView *_chartView;

    Q3DSurface *_surfaceW;
    QWidget *_surfaceContainer;
    Q3DTheme *_theme;
    QSurfaceDataProxy *_3dProxyFunc;
    QSurface3DSeries *_3dSeriesFunc;

    QStackedLayout *_changableLayout;
    QWidget *_activatedContainer, *_deactivatedContainer;

    std::vector<std::shared_ptr<BaseChartItem>> *_charts;
};

#endif // CHARTWIDGET_H
