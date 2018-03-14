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

    _devList.reserve(arr.size());

    for (const auto &iter : arr) {
        QJsonObject obj = iter.toObject();
        // подгрузка свойств
        int in = obj["inputs"].toInt();
        int out = obj["outputs"].toInt();
        int size = 2 * in;
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
        _devList.push_back(Device(in, out, matr));
    }
}

size_t DeviceList::count() const {
    return _devList.size();
}

const Device &DeviceList::operator[](size_t id) const {
    return _devList[id];
}

DeviceList::DeviceList() {}
