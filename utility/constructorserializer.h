#pragma once

#include <QString>
#include <QByteArray>

class ConstructorSerializer {
public:
    static QByteArray serialize();
    static void deserialize(const QByteArray &raw);

    static void save();
    static void load();

    static void setPath(QString path);
    static QString getPath();

private:
    static QString _filename;
};
