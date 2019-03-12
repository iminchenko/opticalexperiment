#ifndef PARAMETERSMANAGER_H
#define PARAMETERSMANAGER_H

#include "ui/mainwindow.h"
#include "utility/singleton.hpp"
#include "command/command.h"

#define PARAM_MANAGER singleton<ParametersManager>()

enum SourcePositionMode {
    OnlyX = 0,
    OnlyY,
    OnCircle,
    InCircle
};

class ParametersManager: public QObject {
    Q_OBJECT
public:
    friend ParametersManager& singleton<ParametersManager>();

    SourcePositionMode getSourcePositionMode() const;
signals:
    void invoke(std::shared_ptr<Command>);
public slots:
    void setSourcePositionMode(int positionMode);

protected:
    ParametersManager();

private:
    SourcePositionMode _positionMode;
};

#endif // PARAMETERSMANAGER_H
