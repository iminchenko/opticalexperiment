#pragma once

#include <QObject>

#include "utility/singleton.hpp"
#include "deviceconfigs/devicedefines.h"

#define INSTRUMENT_CONFIG singleton<InstrumentConfig>()

class InstrumentConfig: public QObject {
    Q_OBJECT
public:
    InstrumentConfig();

    deviceType getType() const;
    int getTypeId() const;

public slots:
    void setType(deviceType type);

    void setTypeLaser();
    void setTypeShield();
    void setTypeGeneric();

private:
    deviceType _type;
    int _itemId;
};
