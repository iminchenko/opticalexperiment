#include "properties.h"


Properties::~Properties() {}

QMap<QString, double> Properties::getProperties() const {
    return _properties;
}

void Properties::setProperties(const QMap<QString, double>& properties) {
    _properties = properties;
}
