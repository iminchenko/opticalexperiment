#include "commandhadlermodel.h"
#include "devicemanager.h"

CommandHadlerModel::CommandHadlerModel() {
    
}

bool CommandHadlerModel::handle(std::shared_ptr<Command> cmnd)
{
    switch (cmnd->typeCommand) {
    case TypeCommand::CMND_ADD_DEVICE:
        return addItem(cmnd);
    case TypeCommand::CMND_ADD_CONNECTION:
        return  addConnection(cmnd);
    case TypeCommand::CMND_DELETE_DEVICE:
        return deleteItem(cmnd);
    case TypeCommand::CMND_REFRESH_DEVICE:
        return false; 
    default:
        return true;
    }
}

bool CommandHadlerModel::addItem(std::shared_ptr<Command> cmnd) {
    if (!cmnd->data.ad.id) {
        cmnd->data.ad.id = DEVICE_MANAGER.getMaxId();
    }
    DEVICE_MANAGER.addDevice(cmnd->data.ad.typeItemId, cmnd->data.ad.id);
    return true;
}

bool CommandHadlerModel::addConnection(std::shared_ptr<Command> cmnd) {
    DEVICE_MANAGER.addConnection(cmnd->data.ac.sourceId,
                                 cmnd->data.ac.sourceNum,
                                 cmnd->data.ac.destId,
                                 cmnd->data.ac.destNum);
    return true;
}

bool CommandHadlerModel::deleteItem(std::shared_ptr<Command> cmnd) {
    DEVICE_MANAGER.removeDevice(cmnd->data.dd.id);
    return true;
}

bool CommandHadlerModel::refreshItem(std::shared_ptr<Command> cmnd) {
    return false;
}
