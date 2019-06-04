#pragma once

#include "ui/basechartitem.h"
#include <QWidget>
#include <QtCharts>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>

using namespace QtDataVisualization;

class ChartWidget : public QScrollArea {
    Q_OBJECT
public:
    ChartWidget(QWidget *parent = nullptr);

    // for chartHandler
    void addChart(int id);
    void removeChart(int id);
    void setChartsArray(std::vector<std::shared_ptr<BaseChartItem>> *charts);
public slots:
    // for combobox
    void updateChart();
    void sourceModeChanged(int sourceMode);
    // for setttings
    void updateSizes();
    void updateScaleFactor();

private:
    void initChart3D(QLayout *containerLayout);
    void initChart2D(QLayout *containerLayout);

    void update3dChart();
    void update2dChart();

    void update3dChart(BaseChartItem *chart);
    void update2dChart(BaseChartItem *chart);
    void updateAreaSizeSettings(BaseChartItem *chart);

    double getXMin() const;
    double getXMax() const;
    double getYMin() const;
    double getYMax() const;

    void deactivate();
    void activate();

    BaseChartItem *getActiveChart();

    QComboBox *_chartSelector;
    QComboBox *_sourceModeSelector;

    QLineEdit *_xMinEditField = nullptr;
    QLineEdit *_xMaxEditField = nullptr;
    QLineEdit *_yMinEditField = nullptr;
    QLineEdit *_yMaxEditField = nullptr;
    QSlider *_slider;
    QLabel *_maxLabel;

    QChart *_chart                  = nullptr;
    Q3DSurface *_surfaceW           = nullptr;
    QSurfaceDataProxy *_3dProxyFunc = nullptr;

    QStackedLayout *_changableLayout;
    QWidget *_activatedContainer;
    QWidget *_deactivatedContainer;

    std::vector<std::shared_ptr<BaseChartItem>> *_charts = nullptr;
};
