#include "parametersmanager.h"
#include "command/commandhanlerglobal.h"

ParametersManager::ParametersManager()
    : _positionMode(SourcePositionMode::OnlyX)
{
    connect(this, SIGNAL(invoke(std::shared_ptr<Command>)),
            &CH_GLOBAL, SLOT(handle(std::shared_ptr<Command>)));
}

SourcePositionMode ParametersManager::getSourcePositionMode() const {
    return _positionMode;
}

void ParametersManager::setSourcePositionMode(int positionMode) {
    _positionMode = static_cast<SourcePositionMode>(positionMode);

    emit invoke(Command::ChangeValues(-1, VarList()));
}
