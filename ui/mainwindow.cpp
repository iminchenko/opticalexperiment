#include <QtCharts>
#include <functional>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "instrumentconfig.h"
#include "propertyobserver.h"
#include "globaldefines.h"
#include "deviceconfigs/deviceconfiglist.h"
#include "commandhandlerview.h"
#include "command/commandhanlerglobal.h"
#include "ui/commandhandlerchart.h"
#include "utility/constructorserializer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    _grInstruments = new QActionGroup(this);

    for (auto iter : ui->instrumentToolBar->actions()) {
        _grInstruments->addAction(iter);
    }
    
    connect(ui->actionLaser, SIGNAL(toggled(bool)),
            InstrumentConfig::i(), SLOT(setTypeLaser()));
    connect(ui->actionShield, SIGNAL(toggled(bool)),
            InstrumentConfig::i(), SLOT(setTypeShield()));
    connect(ui->actionDiffractionGrating, SIGNAL(toggled(bool)),
            InstrumentConfig::i(), SLOT(setTypeDiffractionGrating()));

    auto p = new PropertyObserver(ui->tableWidget, this);

    connect(ui->graphicsView, SIGNAL(propertiesItemClicked(Properties*)),
            p, SLOT(loadProperties(Properties*)));

    initDevices();
    initCommandPattern();

    // TODO: убрать это отседова
    connect(p, SIGNAL(invoke(std::shared_ptr<Command>)),
            CommandHanlerGlobal::i(), SLOT(handle(std::shared_ptr<Command>)));

    ui->graphicsView->resize(this->height() * 4 / 5, 0);
    
    // ToDo: Скорее всего эту строчку куда-то нужно перенести
    CommandHandlerView::i()->setScene(ui->graphicsView->scene()); 
    CommandHandlerChart::i()->setWidget(ui->chartWidget);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initDevices() {
    DeviceConfigList::i()->loadDevices(CONFIG_PATH);

    for (size_t i = 0; i < DeviceConfigList::i()->count(); ++i) {
        QAction *act = new QAction((*DeviceConfigList::i())[i].getName().c_str(),
                                   this);
        act->setProperty("id", QVariant(int(i)));
        act->setCheckable(true);
        _grInstruments->addAction(act);
        ui->instrumentToolBar->insertAction(ui->actionShield, act);
        connect(act, SIGNAL(toggled(bool)),
                InstrumentConfig::i(), SLOT(setTypeGeneric()));

        // icon creation
        // TODO: вынести в отдельный блок
        QImage img((*DeviceConfigList::i())[i].getBounding().size().toSize(),
                   QImage::Format_ARGB32);
        QPainter painter(&img);

        painter.setRenderHint(QPainter::Antialiasing);

        painter.translate(img.width()/2., img.height()/2.);
        painter.scale(0.85, 0.85);

        img.fill(QColor(0, 0, 0, 0));

        (*DeviceConfigList::i())[i].draw(&painter);

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
            CommandHanlerGlobal::i(), SLOT(handle(std::shared_ptr<Command>)));
}

void MainWindow::on_actionSave_triggered() {
    if (ConstructorSerializer::getPath() == "") {
        on_actionSave_As_triggered();
    }

    ConstructorSerializer::save();
}

void MainWindow::on_actionSave_As_triggered() {
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Save as",
                                                    "unnamed.opcstr",
                                                    "*.opcstr");

    if (filename != "") {
        if (filename.endsWith(".opcstr")) {
            filename += ".opcstr";
        }

        ConstructorSerializer::setPath(filename);
        on_actionSave_triggered();
    }
}

void MainWindow::on_actionLoad_triggered() {
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Load",
                                                    "",
                                                    "*.opcstr");

    if (filename != "") {
        ConstructorSerializer::setPath(filename);
        ConstructorSerializer::load();
    }
}
