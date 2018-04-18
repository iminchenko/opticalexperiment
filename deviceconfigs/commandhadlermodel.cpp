#include "commandhadlermodel.h"
#include "devicemanager.h"

CommandHadlerModel::CommandHadlerModel() {
    
}

bool CommandHadlerModel::handler(std::shared_ptr<Command> cmnd)
{
    switch (cmnd->typeCommand()) {
    case TypeCommand::CMND_ADD: 
        return addItem(cmnd);
    case TypeCommand::CMND_DELETE:
        return false;
    case TypeCommand::CMND_REFRESH:
        return false; 
    default:
        return true;
    }
}

bool CommandHadlerModel::addItem(std::shared_ptr<Command> cmnd) {
    DEVICE_MANAGER.addDevice(cmnd->typeItemId(), cmnd->id());
    return true;
}
