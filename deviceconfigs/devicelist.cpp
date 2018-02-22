#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QByteArray>
#include <QFile>
#include <QDebug>

#include "devicelist.h"

using std::complex;
using std::string;

void DeviceList::loadDevices(std::string filename) {
    QJsonDocument doc;
    QByteArray raw;
    // возможно стоить принимать сразу QString?
    QFile file(filename.c_str());

    if (file.open(QIODevice::ReadOnly)) {
        while (!file.atEnd()) {
            raw.append(file.readLine());
        }
    }

    QJsonParseError err;

    doc = QJsonDocument::fromJson(raw, &err);

    if (err.error != QJsonParseError::NoError) {
        // на самом деле надо бы кидать исключение и сообщать
        // об этом в окошке
        qDebug() << err.errorString() << err.offset;
    }

    QJsonArray arr = doc.array();

    _devList.resize(arr.size());

    for (int i = 0; i < arr.size(); ++i) {
        // подгрузка свойств
    }
}

const Device &DeviceList::operator[](size_t id) const {
    return _devList[id];
}

DeviceList::DeviceList() {

}
