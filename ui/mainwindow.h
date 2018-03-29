#pragma once

#include <QMainWindow>
#include <QActionGroup>
#include <QtCharts>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow() override;

private slots:
    void onDeviceEmplacementChanged();

private:
    void initDevices();
    void initCharts();

    Ui::MainWindow *ui;
    QActionGroup *_grInstruments;
    // TODO: мэнэджментом дисплеев должен заниматься не `MainWindow`
    QChart *_chart;
};
