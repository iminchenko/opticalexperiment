#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "instrumentconfig.h"
#include "propertyobserver.h"
#include "globaldefines.h"
#include "deviceconfigs/deviceviewlist.h"


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
    connect(ui->actionOpticalGrating, SIGNAL(toggled(bool)),
            &INSTRUMENT_CONFIG, SLOT(setTypeOpticalGrating()));
    connect(ui->actionShield, SIGNAL(toggled(bool)),
            &INSTRUMENT_CONFIG, SLOT(setTypeShield()));
    connect(ui->actionLightBlower, SIGNAL(toggled(bool)),
            &INSTRUMENT_CONFIG, SLOT(setTypeLightblower()));
    connect(ui->actionLambda_2, SIGNAL(toggled(bool)),
            &INSTRUMENT_CONFIG, SLOT(setTypeLambda2()));
    connect(ui->actionLambda_4, SIGNAL(toggled(bool)),
            &INSTRUMENT_CONFIG, SLOT(setTypeLambda4()));
    connect(ui->actionPolarizer, SIGNAL(toggled(bool)),
            &INSTRUMENT_CONFIG, SLOT(setTypePolarizer()));
    connect(ui->actionPhaseModulator, SIGNAL(toggled(bool)),
            &INSTRUMENT_CONFIG, SLOT(setTypePhaseModulator()));

    auto p = new PropertyObserver(ui->tableWidget, this);

    connect(ui->graphicsView, SIGNAL(propertiesItemClicked(Properties*)),
            p, SLOT(loadProperties(Properties*)));

    initDevices();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initDevices() {
    DEVICEVIEW_LIST.loadDevices(CONFIG_PATH);

    for (size_t i = 0; i < DEVICEVIEW_LIST.count(); ++i) {
        QAction *act = new QAction(DEVICEVIEW_LIST[i].getName().c_str(), this);
        act->setProperty("id", QVariant(int(i)));
        act->setCheckable(true);
        _grInstruments->addAction(act);
        ui->instrumentToolBar->insertAction(ui->actionShield, act);
        connect(act, SIGNAL(toggled(bool)),
                &INSTRUMENT_CONFIG, SLOT(setTypeGeneric()));
    }
    ui->instrumentToolBar->insertSeparator(ui->actionShield);
}
