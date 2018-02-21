#pragma once

#include <QObject>

#include "singleton.hpp"

#define INSTRUMENT_CONFIG singleton<InstrumentConfig>()

class InstrumentConfig: public QObject{
    Q_OBJECT
public:
    enum instrumentType {
        TYPE_LASER = 0,
        TYPE_OPTICALGRATING,
        TYPE_LIGHTBLOWER,
        TYPE_LAMBDA2,
        TYPE_LAMBDA4,
        TYPE_POLARIZER,
        TYPE_PHASEMODULATOR,
        TYPE_SHIELD
    };

    InstrumentConfig();

    instrumentType getType() const;

public slots:
    void setType(instrumentType type);

    void setTypeLaser();
    void setTypeOpticalGrating();
    void setTypeLightblower();
    void setTypeShield();
    void setTypeLambda2();
    void setTypeLambda4();
    void setTypePolarizer();
    void setTypePhaseModulator();

private:
    instrumentType _type;
};
