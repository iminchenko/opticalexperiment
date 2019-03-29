#include "commandhadlermodel.h"
#include "devicemanager.h"
#include "utility/idgenerator.h"

CommandHadlerModel::CommandHadlerModel() {}

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
        cmnd->data.ad.id = DEVICE_ID_GEN.getId();
    } else if (DEVICE_ID_GEN.getLastId() < cmnd->data.ad.id) {
        DEVICE_ID_GEN.setLastId(cmnd->data.ad.id);
    }

    if (DEVICE_MANAGER.getDeviceById(cmnd->data.ad.id)) {
        throw std::logic_error("Device with id already exists");
    }

    DEVICE_MANAGER.addDevice(cmnd->data.ad.typeItemId, cmnd->data.ad.id);
    return true;
}

bool CommandHadlerModel::addConnection(pCommand cmnd) {
    DEVICE_MANAGER.addConnection(cmnd->data.ac.sourceId,
                                 cmnd->data.ac.sourceNum,
                                 cmnd->data.ac.destId,
                                 cmnd->data.ac.destNum);
    return true;
}

bool CommandHadlerModel::deleteItem(pCommand cmnd) {
    DEVICE_MANAGER.removeDevice(cmnd->data.dd.id);
    return true;
}

bool CommandHadlerModel::deleteConnection(pCommand cmnd) {
    DEVICE_MANAGER.removeConnection(cmnd->data.dc.sourceId,
                                    cmnd->data.dc.sourceNum,
                                    cmnd->data.dc.destId,
                                    cmnd->data.dc.destNum);
    return true;
}

bool CommandHadlerModel::changeVariables(pCommand cmnd) {
    DEVICE_MANAGER.changeVariables(cmnd->data.cv.id, cmnd->varList);
    return true;
}

bool CommandHadlerModel::refreshItem(pCommand cmnd) {
    Q_UNUSED(cmnd);
    return true;
}
