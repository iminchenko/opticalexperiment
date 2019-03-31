#ifndef PARAMETERSMANAGER_H
#define PARAMETERSMANAGER_H

#include "ui/mainwindow.h"
#include "utility/singleton.hpp"
#include "command/command.h"

enum SourcePositionMode {
    OnlyX = 0,
    OnlyY,
    OnCircle,
    InCircle
};

class ParametersManager: public QObject, public Singleton<ParametersManager> {
    Q_OBJECT
public:
    ParametersManager();

    SourcePositionMode getSourcePositionMode() const;
signals:
    void invoke(std::shared_ptr<Command>);
public slots:
    void setSourcePositionMode(int positionMode);

private:
    SourcePositionMode _positionMode;
};

#endif // PARAMETERSMANAGER_H
