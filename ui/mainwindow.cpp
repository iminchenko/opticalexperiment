#include <QtCharts>
#include <functional>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "instrumentconfig.h"
#include "propertyobserver.h"
#include "globaldefines.h"
#include "deviceconfigs/deviceconfiglist.h"
#include "devicemanager.h"
#include "commandhandlerview.h"
#include "command/commandhanlerglobal.h"
#include "ui/commandhandlerchart.h"

#include "qcombobox.h"

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
    connect(ui->actionDiffractionGrating, SIGNAL(toggled(bool)),
            &INSTRUMENT_CONFIG, SLOT(setTypeDiffractionGrating()));

    auto p = new PropertyObserver(ui->tableWidget, this);

    connect(ui->graphicsView, SIGNAL(propertiesItemClicked(Properties*)),
            p, SLOT(loadProperties(Properties*)));

    initDevices();
    initCommandPattern();

    // TODO убрать это отседова
    connect(p, SIGNAL(invoke(std::shared_ptr<Command>)),
            &CH_GLOBAL, SLOT(handle(std::shared_ptr<Command>)));

    ui->graphicsView->resize(this->height() * 4 / 5, 0);
    
    // ToDo: Скорее всего эту строчку куда-то нужно перенести
    CH_VIEW.setScene(ui->graphicsView->scene()); 
    CH_CHART.setWidget(ui->chartWidget);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initDevices() {
    DEVICECONFIG_LIST.loadDevices(CONFIG_PATH);

    for (size_t i = 0; i < DEVICECONFIG_LIST.count(); ++i) {
        QAction *act = new QAction(DEVICECONFIG_LIST[i].getName().c_str(),
                                   this);
        act->setProperty("id", QVariant(int(i)));
        act->setCheckable(true);
        _grInstruments->addAction(act);
        ui->instrumentToolBar->insertAction(ui->actionShield, act);
        connect(act, SIGNAL(toggled(bool)),
                &INSTRUMENT_CONFIG, SLOT(setTypeGeneric()));

        // icon creation
        // TODO: вынести в отдельный блок
        QImage img(DEVICECONFIG_LIST[i].getBounding().size().toSize(),
                   QImage::Format_ARGB32);
        QPainter painter(&img);

        painter.setRenderHint(QPainter::Antialiasing);

        painter.translate(img.width()/2., img.height()/2.);
        painter.scale(0.85, 0.85);

        img.fill(QColor(0, 0, 0, 0));

        DEVICECONFIG_LIST[i].draw(&painter);

        QPixmap pix;
        pix.convertFromImage(img);
        act->setIcon(QIcon(pix));
    }

    ui->instrumentToolBar->insertSeparator(ui->actionShield);
}

void MainWindow::showInfoWindow() {
    QMessageBox mesbox(this);
    mesbox.setWindowTitle("About");
    mesbox.setText("This is simulation of a physical experiment<br><br>"
                   "<a href='https://bitbucket.org/iminchenko/opticalexperiment'>Repository</a><br><br>"
                   "Autors:<ul>"
                   "<li>Ilia Minchenko - developer</li>"
                   "<li>Roman Larin - developer</li>"
                   "<li>Andrey Kozy - mathematician</li>"
                   "<li>Evgeniy Kiktenko - research supervisor</li></ul>");
    mesbox.setTextFormat(Qt::RichText);
    mesbox.exec();
}

void MainWindow::initCommandPattern() {
    connect(ui->graphicsView, SIGNAL(invoke(std::shared_ptr<Command>)),
            &CH_GLOBAL, SLOT(handle(std::shared_ptr<Command>)));
}
