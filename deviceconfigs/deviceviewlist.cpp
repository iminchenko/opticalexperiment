#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <vector>

#include "deviceviewlist.h"

using std::vector;

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

    _devList.resize(arr.size());

    for (int i = 0; i < arr.size(); ++i) {
        QJsonObject obj = arr[i].toObject();

        QJsonArray drawArray = obj["drawing"].toArray();

        vector<DrawingConfig> drawing;
        drawing.reserve(drawArray.size());

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
                oneDrawing.text = drawObj["text"].toString().toStdString();
            }

            drawing.push_back(oneDrawing);
        }

        _devList[i] = DeviceViewConfig(obj["name"].toString().toStdString(),
                obj["description"].toString().toStdString(), drawing);
    }
}

const DeviceViewConfig &DeviceViewList::operator[](size_t id) const {
    return _devList[id];
}

DeviceViewList::DeviceViewList() {}
