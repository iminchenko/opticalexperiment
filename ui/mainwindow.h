#pragma once

#include <QMainWindow>

class QActionGroup;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initDevices();

    Ui::MainWindow *ui;
    QActionGroup *_grInstruments;
};
