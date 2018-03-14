#include <QtCharts>
#include <QtCharts/QChartView>
#include <QSpacerItem>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "instrumentconfig.h"
#include "propertyobserver.h"
#include "globaldefines.h"
#include "deviceconfigs/deviceviewlist.h"
#include "deviceconfigs/devicelist.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    _grInstruments = new QActionGroup(this);

    for (auto iter : ui->instrumentToolBar->actions()) {
        _grInstruments->addAction(iter);
    }

    connect(ui->actionLaser, SIGNAL(toggled(bool)),
            &INSTRUMENT_CONFIG, SLOT(setTypeLaser()));
    connect(ui->actionShield, SIGNAL(toggled(bool)),
            &INSTRUMENT_CONFIG, SLOT(setTypeShield()));

    auto p = new PropertyObserver(ui->tableWidget, this);

    connect(ui->graphicsView, SIGNAL(propertiesItemClicked(Properties*)),
            p, SLOT(loadProperties(Properties*)));

    initDevices();
    initCharts();

    ui->graphicsView->resize(this->height() * 4 / 5, 0);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initDevices() {
    DEVICEVIEW_LIST.loadDevices(CONFIG_PATH);
    DEVICE_LIST.loadDevices(CONFIG_PATH);

    for (size_t i = 0; i < DEVICEVIEW_LIST.count(); ++i) {
        QAction *act = new QAction(DEVICEVIEW_LIST[i].getName().c_str(), this);
        act->setProperty("id", QVariant(int(i)));
        act->setCheckable(true);
        _grInstruments->addAction(act);
        ui->instrumentToolBar->insertAction(ui->actionShield, act);
        connect(act, SIGNAL(toggled(bool)),
                &INSTRUMENT_CONFIG, SLOT(setTypeGeneric()));

        // icon creation
        // TODO: вынести в отдельный блок
        QImage img(DEVICEVIEW_LIST[i].getBounding().size().toSize(), QImage::Format_ARGB32);
        QPainter painter(&img);

        painter.setRenderHint(QPainter::Antialiasing);

        painter.translate(img.width()/2, img.height()/2);
        painter.scale(0.85, 0.85);

        img.fill(QColor(0, 0, 0, 0));

        DEVICEVIEW_LIST[i].draw(&painter);

        QPixmap pix;
        pix.convertFromImage(img);
        act->setIcon(QIcon(pix));
    }
    ui->instrumentToolBar->insertSeparator(ui->actionShield);
}

void MainWindow::initCharts() {
    QChart *chart = new QChart();
    chart->setTitle("Screen chart");

    chart->addSeries(new QLineSeries(chart));
    chart->createDefaultAxes();
    chart->legend()->hide();
    chart->axisX()->setRange(-1, 1);
    chart->axisY()->setRange(0, 1);

    ui->leftLayout->insertWidget(1, new QChartView(chart));
}
