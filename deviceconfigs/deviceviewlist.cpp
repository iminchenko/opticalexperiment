#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <list>

#include "deviceviewlist.h"

using std::list;

void DeviceViewList::loadDevices(std::string filename) {
    QJsonDocument doc;
    QByteArray raw;
    QFile file(filename.c_str());

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open file" << filename.c_str();
    }

    while (!file.atEnd()) {
        raw.append(file.readLine());
    }

    QJsonParseError err;

    doc = QJsonDocument::fromJson(raw, &err);

    if (err.error != QJsonParseError::NoError) {
        // на самом деле надо бы кидать исключение и сообщать
        // об этом в окошке
        qDebug() << err.errorString() << err.offset;

        QString str;
        for (int i = 0; i < err.offset; ++i)
            str.push_back(QChar(raw[i]));
        qDebug() << raw;
        qDebug() << str;
    }

    QJsonArray arr = doc.array();

    _devList.reserve(arr.size());

    for (const auto &iter : arr) {
        QJsonObject obj = iter.toObject();

        QJsonArray drawArray = obj["drawing"].toArray();

        list<DrawingConfig> drawing;

        for (const auto &item : drawArray) {
            QJsonValue drawObj = item.toObject();
            DrawingConfig oneDrawing;
            DrawingConfig::drawingType type =
                DrawingConfig::toType(drawObj["type"].toString().toStdString());

            oneDrawing.type = type;

            if (type != DrawingConfig::TYPE_TEXT) {
                QJsonArray jsonCoords = drawObj["coordinates"].toArray();
                oneDrawing.coordinates.reserve(jsonCoords.size());

                for (const auto &coordinate : jsonCoords) {
                    oneDrawing.coordinates.push_back(coordinate.toInt());
                }
            }
            else {
                oneDrawing.text = drawObj["data"].toString().toStdString();
            }

            drawing.push_back(oneDrawing);
        }

        int inputCount = obj["inputs"].toInt();
        int outputCount = obj["outputs"].toInt();

        _devList.push_back(DeviceViewConfig(inputCount, outputCount,
                        obj["name"].toString().toStdString(),
                        obj["description"].toString().toStdString(), drawing));
    }
}

size_t DeviceViewList::count() const {
    return _devList.size();
}

const DeviceViewConfig &DeviceViewList::operator[](size_t id) const {
    return _devList[id];
}

DeviceViewList::DeviceViewList() {}
