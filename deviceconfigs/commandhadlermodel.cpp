#include "commandhadlermodel.h"
#include "devicemanager.h"

CommandHadlerModel::CommandHadlerModel()
{
    
}

bool CommandHadlerModel::addItem(std::shared_ptr<Command> cmnd)
{
    DEVICE_MANAGER.addDevice(cmnd->typeItemId(), cmnd->id());
    return true;
}
