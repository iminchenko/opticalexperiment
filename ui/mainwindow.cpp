#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "instrumentconfig.h"
#include "propertyobserver.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QActionGroup *grInstruments = new QActionGroup(this);

    for (auto iter : ui->instrumentToolBar->actions()) {
        grInstruments->addAction(iter);
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
}

MainWindow::~MainWindow() {
    delete ui;
}
