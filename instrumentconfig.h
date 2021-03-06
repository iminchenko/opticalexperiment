#pragma once

#include <QObject>

#include "utility/singleton.hpp"
#include "deviceconfigs/devicedefines.h"

class InstrumentConfig: public QObject, public Singleton<InstrumentConfig> {
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
    void setTypeDiffractionGrating();

private:
    deviceType _type;
    int _itemId;
};
