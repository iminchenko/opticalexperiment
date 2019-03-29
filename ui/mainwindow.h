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

private slots:
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionLoad_triggered();

private:
    void initDevices();
    void initCommandPattern();

    Ui::MainWindow *ui;
    QActionGroup *_grInstruments;
};
