#pragma once

#include <QObject>

#include "utility/singleton.hpp"

#define INSTRUMENT_CONFIG singleton<InstrumentConfig>()

class InstrumentConfig: public QObject{
    Q_OBJECT
public:
    enum instrumentType {
        TYPE_LASER = -1,
        TYPE_SHIELD = -2,
        TYPE_GENERIC = 0
    };

    InstrumentConfig();

    instrumentType getType() const;
    int getItemId() const;

public slots:
    void setType(instrumentType type);

    void setTypeLaser();
    void setTypeShield();
    void setTypeGeneric();

private:
    instrumentType _type;
    int _itemId;
};
