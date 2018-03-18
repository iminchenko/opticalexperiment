#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <list>

#include "deviceconfiglist.h"

using std::list;
using std::complex;

void DeviceConfigList::loadDevices(std::string filename) {
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

        int size = 2 * inputCount;
        Matrix<complex<double>> matr(size, size);
        QJsonArray rows = obj["matrix"].toArray();
        for (int i = 0; i < size; ++i) {
            QJsonArray row = rows[i].toArray();
            for (int j = 0; j < size; ++j) {
                complex<double> temp(row[j].toArray()[0].toDouble(),
                                     row[j].toArray()[1].toDouble());
                matr[i][j] = temp;
            }
        }

        _devList.push_back(DeviceConfig(inputCount, outputCount,
                        obj["name"].toString().toStdString(),
                        obj["description"].toString().toStdString(), drawing,
                        matr));
    }
}

size_t DeviceConfigList::count() const {
    return _devList.size();
}

const DeviceConfig &DeviceConfigList::operator[](size_t id) const {
    // позже добавить нормальную проверку на лазер и экран
    if (id == deviceType::TYPE_LASER || id == deviceType::TYPE_SHIELD)
        return _devList[0];

    return _devList[id];
}

DeviceConfigList::DeviceConfigList() {}