#include "commandhadlermodel.h"
#include "devicemanager.h"

CommandHadlerModel::CommandHadlerModel() {
    
}

bool CommandHadlerModel::handle(std::shared_ptr<Command> cmnd)
{
    switch (cmnd->typeCommand) {
    case TypeCommand::CMND_ADD_DEVICE:
        return addItem(cmnd);
    case TypeCommand::CMND_DELETE_DEVICE:
        return false;
    case TypeCommand::CMND_REFRESH_DEVICE:
        return false; 
    default:
        return true;
    }
}

bool CommandHadlerModel::addItem(std::shared_ptr<Command> cmnd) {
    cmnd->data.ad.id = DEVICE_MANAGER.getMaxId();
    DEVICE_MANAGER.addDevice(cmnd->data.ad.typeItemId, cmnd->data.ad.id);
    return true;
}
