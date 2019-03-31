#include "parametersmanager.h"
#include "command/commandhanlerglobal.h"

ParametersManager::ParametersManager()
      : QObject(nullptr),
        Singleton<ParametersManager>(*this),
        _positionMode(SourcePositionMode::OnlyX) {
    connect(this, SIGNAL(invoke(std::shared_ptr<Command>)),
            CommandHanlerGlobal::i(), SLOT(handle(std::shared_ptr<Command>)));
}

SourcePositionMode ParametersManager::getSourcePositionMode() const {
    return _positionMode;
}

void ParametersManager::setSourcePositionMode(int positionMode) {
    _positionMode = static_cast<SourcePositionMode>(positionMode);

    emit invoke(Command::ChangeValues(-1, VarList()));
}
