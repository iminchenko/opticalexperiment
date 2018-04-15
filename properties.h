#pragma once

#include <QMap>
#include <QString>

class Properties {
public:
    using map = QMap<QString, double>;

    virtual QMap<QString, double> getProperties() const;
    virtual void setProperties(const map &properties);

private:
    map _properties;
};
