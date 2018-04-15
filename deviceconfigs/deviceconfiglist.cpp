#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <list>

#include "deviceconfiglist.h"
#include "globaldefines.h"
#include "utility/parser.h"

using std::list;
using std::complex;

void DeviceConfigList::loadDevices(std::string filename) {
    QJsonDocument doc;
    QByteArray raw;
    QFile file(filename.c_str());

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open file" << filename.c_str();
        return;
    }

    while (!file.atEnd()) {
        raw.append(file.readLine());
    }

    QJsonParseError err{};

    doc = QJsonDocument::fromJson(raw, &err);

    if (err.error != QJsonParseError::NoError) {
#ifdef _DEBUG
        // на самом деле надо бы кидать исключение и сообщать
        // об этом в окошке
        qDebug() << err.errorString() << err.offset;

        QString str;
        for (int i = 0; i < err.offset; ++i)
            str.push_back(QChar(raw[i]));
        qDebug() << raw;
        qDebug() << str;
#endif
    }

    QJsonArray arr = doc.array();

    _devList.reserve((size_t)arr.size());

    for (const auto &iter : arr) {
        QJsonObject obj = iter.toObject();

        QJsonArray drawArray = obj["drawing"].toArray();

        list<DrawingConfig> drawing;

        for (const auto &item : drawArray) {
            QJsonObject drawObj = item.toObject();
            DrawingConfig oneDrawing;
            DrawingConfig::drawingType type =
                DrawingConfig::toType(drawObj["type"].toString().toStdString());

            oneDrawing.type = type;

            if (type != DrawingConfig::TYPE_TEXT) {
                QJsonArray jsonCoords = drawObj["coordinates"].toArray();
                oneDrawing.coordinates.reserve((size_t)jsonCoords.size());

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
        QJsonArray rows = obj["matrix"].toArray();
        ExprMatrix matr(rows.size(), size);
        for (int i = 0; i < rows.size(); ++i) {
            QJsonArray row = rows[i].toArray();
            for (int j = 0; j < row.size(); ++j) {
                const auto &item = row[j].toArray();

                matr[i][j].first = (item[0].isDouble() ?
                    QString::number(item[0].toDouble()) : item[0].toString()
                    ).toStdString();
                matr[i][j].second = (item[1].isDouble() ?
                    QString::number(item[1].toDouble()) : item[1].toString()
                    ).toStdString();

                insertMathConsts(matr[i][j].first);
                insertMathConsts(matr[i][j].second);
            }
        }

        VarList variables;

        if (obj.contains("variables")) {
            QJsonObject varObj = obj["variables"].toObject();
            for (const auto& key : varObj.keys()) {
                variables.emplace_back(key.toStdString(), varObj[key].toDouble());
            }
        }

        _devList.emplace_back(inputCount, outputCount,
                        obj["name"].toString().toStdString(),
                        obj["description"].toString().toStdString(), drawing,
                        variables, matr);
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
