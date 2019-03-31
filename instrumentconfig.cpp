#include <QVariant>

#include "instrumentconfig.h"

InstrumentConfig::InstrumentConfig()
     : QObject(nullptr),
       Singleton<InstrumentConfig>(*this),
       _type(TYPE_LASER) {}

deviceType InstrumentConfig::getType() const {
    return _type;
}

int InstrumentConfig::getTypeId() const {
    return _itemId;
}

void InstrumentConfig::setType(deviceType type) {
    _type = type;
    _itemId = type;
}

void InstrumentConfig::setTypeLaser() {
    setType(TYPE_LASER);
}

void InstrumentConfig::setTypeShield() {
    setType(TYPE_SHIELD);
}

void InstrumentConfig::setTypeGeneric() {
    setType(TYPE_GENERIC);

    QObject* obj = sender();

    if (obj && obj->property("id").isValid()) {
        _itemId = obj->property("id").toInt();
    }
}
