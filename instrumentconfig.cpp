#include <QVariant>

#include "instrumentconfig.h"

InstrumentConfig::InstrumentConfig()
    :_type(TYPE_LASER) {}

InstrumentConfig::instrumentType InstrumentConfig::getType() const {
    return _type;
}

int InstrumentConfig::getItemId() const {
    return _itemId;
}

void InstrumentConfig::setType(instrumentType type) {
    _type = type;
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
