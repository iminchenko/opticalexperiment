#include <QtCharts>
#include <functional>
#include <memory>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

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
#include "ui/parametersmanager.h"
#include "utility/idgenerator.h"
#include "devicemanager.h"
#include "deviceconfigs/device.h"

#include "qcombobox.h"

using std::list;
using std::shared_ptr;

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
    ui->cbMode->addItems(QStringList() << "(x, 0)" << "(0, x)" << "On Circle" << "In Circle");
    connect( ui->cbMode, SIGNAL(currentIndexChanged(int)),
            &PARAM_MANAGER, SLOT(setSourcePositionMode(int)));

    initDevices();
    initCommandPattern();

    // TODO убрать это отседова
    connect(p, SIGNAL(invoke(std::shared_ptr<Command>)),
            &CH_GLOBAL, SLOT(handle(std::shared_ptr<Command>)));

    ui->graphicsView->resize(this->height() * 4 / 5, 0);
    
    // ToDo: Скорее всего эту строчку куда-то нужно перенести
    CH_VIEW.setScene(ui->graphicsView->scene()); 
    CH_CHART.setTabWidget(ui->tabWidget);
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

void MainWindow::on_actionSave_triggered(){
    int lastId = DEVICE_ID_GEN.getLastId();

    list<shared_ptr<Device>> devices;

    QJsonArray jsonArray;

    for (int i = 0; i < lastId; ++i) {
        if (auto device = DEVICE_MANAGER.getDeviceById(i)) {
            QJsonObject jsonDevice;

            jsonDevice["id"] = device->getId();
            jsonDevice["type"]= device->getType();

            QPointF pos = CH_VIEW.getDevicePos(device->getId());

            jsonDevice["pos"] = QJsonArray{ pos.x(), pos.y() };

            auto varList = device->getVariables();
            if (varList.size()) {
                QJsonObject varsObject;

                for (const auto &variable: varList) {
                    varsObject[variable.first.c_str()] = variable.second;
                }

                jsonDevice["variables"] = varsObject;
            }

            auto config = DEVICECONFIG_LIST[device->getType()];
            QJsonArray connections;
            for (int j = 0; j < config.getInputCount(); ++j) {
                if (auto connectedDevice = device->getConnection(j).device.lock()) {
                    QJsonObject connection;

                    connection["sourceDevId"] = connectedDevice->getId();
                    connection["destDevId"] = device->getId();
                    connection["sourceOutput"] = device->getConnection(j).output;
                    connection["destInput"] = j;

                    connections.push_back(connection);
                }
            }

            if (!connections.empty()) {
                jsonDevice["connections"] = connections;
            }

            jsonArray.push_back(jsonDevice);
        }
    }

    QJsonDocument doc(jsonArray);

    QString filename = "saved.json";

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Can't open file" << filename;
        return;
    }

    file.write(doc.toJson());
}

void MainWindow::on_actionLoad_triggered() {
    QString filename = "saved.json";

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open file" << filename;
        return;
    }

    QByteArray raw = file.readAll();

    QJsonParseError err{};

    auto doc = QJsonDocument::fromJson(raw, &err);

    if (err.error != QJsonParseError::NoError) {
#ifdef _DEBUG
        qDebug() << err.errorString() << err.offset;

        QString str;
        for (int i = 0; i < err.offset; ++i) {
            str.push_back(QChar(raw[i]));
        }
        qDebug() << raw;
        qDebug() << str;
#endif
    }

    QJsonArray devices = doc.array();

    for (auto device: devices) {
        auto deviceObject = device.toObject();
        auto posArray = deviceObject["pos"].toArray();

        auto command = Command::AddDevice(QPointF(posArray[0].toDouble(),
                                                  posArray[1].toDouble()),
                                          deviceObject["type"].toInt(),
                                          deviceObject["id"].toInt());

        CH_GLOBAL.handle(command);

        if (deviceObject.contains("variables")) {
            auto varsObject = deviceObject["variables"].toObject();

            VarList varList;

            for (auto key: varsObject.keys()) {
                varList.emplace_back(key.toStdString().c_str(),
                                     varsObject[key].toDouble());
            }

            command = Command::ChangeValues(deviceObject["id"].toInt(), varList);
            CH_GLOBAL.handle(command);
        }
    }
}
