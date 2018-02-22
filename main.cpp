#include "ui/mainwindow.h"
#include <QApplication>

#include "deviceconfigs/deviceviewlist.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    DEVICEVIEW_LIST.loadDevices("devices.json");

    return a.exec();
}
