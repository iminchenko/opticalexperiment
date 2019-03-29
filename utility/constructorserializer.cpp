#include <list>
#include <memory>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

#include "constructorserializer.h"
#include "utility/idgenerator.h"
#include "deviceconfigs/device.h"
#include "devicemanager.h"
#include "ui/commandhandlerview.h"
#include "deviceconfigs/deviceconfiglist.h"
#include "command/commandhanlerglobal.h"

QString ConstructorSerializer::_filename = "saved.json";

QByteArray ConstructorSerializer::serialize() {
    int lastId = DEVICE_ID_GEN.getLastId();

    std::list<std::shared_ptr<Device>> devices;

    QJsonArray jsonArray;

    for (int i = 0; i <= lastId; ++i) {
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
            int inputCount = config.getInputCount();
            if (device->getType() == deviceType::TYPE_SHIELD) {
                inputCount = 1;
            }
            for (int j = 0; j < inputCount; ++j) {
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

    return QJsonDocument(jsonArray).toJson();
}

void ConstructorSerializer::deserialize(const QByteArray &raw) {
    // Удаление всех устройств
    for (int i = 0; i <= DEVICE_ID_GEN.getLastId(); ++i) {
        auto command = Command::DeleteDevice(i);
        CH_GLOBAL.handle(command);
    }
    DEVICE_ID_GEN.setLastId(NO_ID);

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

    std::list<QJsonObject> connections;

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

        if (deviceObject.contains("connections")) {
            for (auto connection: deviceObject["connections"].toArray()) {
                connections.push_back(connection.toObject());
            }
        }
    }

    for (auto connection: connections) {
        auto command = Command::AddConnection(
                                        connection["sourceDevId"].toInt(),
                                        connection["destDevId"].toInt(),
                                        connection["sourceOutput"].toInt(),
                                        connection["destInput"].toInt());
        CH_GLOBAL.handle(command);
    }
}

void ConstructorSerializer::save() {
    QString filename = "saved.json";

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Can't open file" << filename;
        return;
    }

    file.write(serialize());
}

void ConstructorSerializer::load() {
    QFile file(_filename);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open file" << _filename;
        return;
    }

    QByteArray raw = file.readAll();

    ConstructorSerializer::deserialize(raw);
}

void ConstructorSerializer::setPath(QString path) {
    _filename = path;
}

QString ConstructorSerializer::getPath() {
    return _filename;
}
