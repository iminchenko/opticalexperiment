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

public slots:
    void showInfoWindow();

private:
    void initDevices();
    void initCommandPattern();

    Ui::MainWindow *ui;
    QActionGroup *_grInstruments;
};
