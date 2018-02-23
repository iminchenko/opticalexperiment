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

void InstrumentConfig::setTypeOpticalGrating() {
    setType(TYPE_OPTICALGRATING);
}

void InstrumentConfig::setTypeLightblower() {
    setType(TYPE_LIGHTBLOWER);
}

void InstrumentConfig::setTypeShield() {
    setType(TYPE_SHIELD);
}

void InstrumentConfig::setTypeLambda2() {
    setType(TYPE_LAMBDA2);
}

void InstrumentConfig::setTypeLambda4() {
    setType(TYPE_LAMBDA4);
}

void InstrumentConfig::setTypePolarizer() {
    setType(TYPE_POLARIZER);
}

void InstrumentConfig::setTypePhaseModulator() {
    setType(TYPE_PHASEMODULATOR);
}

void InstrumentConfig::setTypeGeneric() {
    setType(TYPE_GENERIC);

    QObject* obj = sender();

    if (obj && obj->property("id").isValid()) {
        _itemId = obj->property("id").toInt();
    }
}
