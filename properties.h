#pragma once

#include <QMap>
#include <QString>

class Properties {
public:
    using map = QMap<QString, double>;

    virtual QMap<QString, double> getProperties() const;
    virtual void setProperties(const map &properties);

    // TODO: костыль неидеальной системы
    virtual int getId() const = 0;

private:
    map _properties;
};
