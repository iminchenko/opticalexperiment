#pragma once

#include <QMainWindow>
#include <QActionGroup>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initDevices();
    void initCharts();

    Ui::MainWindow *ui;
    QActionGroup *_grInstruments;
};
