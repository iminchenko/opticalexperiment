#include "commandhadlermodel.h"
#include "devicemanager.h"
#include "utility/idgenerator.h"

CommandHadlerModel::CommandHadlerModel() 
     : CommandHandler(nullptr),
       Singleton<CommandHadlerModel> (*this) {}

bool CommandHadlerModel::handle(pCommand cmnd) {
    switch (cmnd->typeCommand) {
    case TypeCommand::CMND_ADD_DEVICE:
        return addItem(cmnd);
    case TypeCommand::CMND_ADD_CONNECTION:
        return  addConnection(cmnd);
    case TypeCommand::CMND_DELETE_DEVICE:
        return deleteItem(cmnd);
    case TypeCommand::CMND_DELETE_CONNECTION:
        return deleteConnection(cmnd);
    case TypeCommand::CMND_REFRESH_DEVICE:
        return false;
    case TypeCommand ::CMND_CHANGE_VARIABLE:
        return changeVariables(cmnd);
    default:
        return true;
    }
}

bool CommandHadlerModel::addItem(pCommand cmnd) {
    if (cmnd->data.ad.id == -1) {
        cmnd->data.ad.id = IdGenerator::i()->getId();
    } else if (IdGenerator::i()->getLastId() < cmnd->data.ad.id) {
        IdGenerator::i()->setLastId(cmnd->data.ad.id);
    }

    if (DeviceManager::i()->getDeviceById(cmnd->data.ad.id)) {
        throw std::logic_error("Device with id already exists");
    }

    DeviceManager::i()->addDevice(cmnd->data.ad.typeItemId, cmnd->data.ad.id);
    return true;
}

bool CommandHadlerModel::addConnection(pCommand cmnd) {
    DeviceManager::i()->addConnection(cmnd->data.ac.sourceId,
                                 cmnd->data.ac.sourceNum,
                                 cmnd->data.ac.destId,
                                 cmnd->data.ac.destNum);
    return true;
}

bool CommandHadlerModel::deleteItem(pCommand cmnd) {
    DeviceManager::i()->removeDevice(cmnd->data.dd.id);
    return true;
}

bool CommandHadlerModel::deleteConnection(pCommand cmnd) {
    DeviceManager::i()->removeConnection(cmnd->data.dc.sourceId,
                                    cmnd->data.dc.sourceNum,
                                    cmnd->data.dc.destId,
                                    cmnd->data.dc.destNum);
    return true;
}

bool CommandHadlerModel::changeVariables(pCommand cmnd) {
    DeviceManager::i()->changeVariables(cmnd->data.cv.id, cmnd->varList);
    return true;
}

bool CommandHadlerModel::refreshItem(pCommand cmnd) {
    Q_UNUSED(cmnd);
    return true;
}
