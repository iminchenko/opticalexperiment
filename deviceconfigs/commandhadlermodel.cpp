#include "commandhadlermodel.h"
#include "devicemanager.h"

CommandHadlerModel::CommandHadlerModel()
{
    
}

bool CommandHadlerModel::addItem(int type, int id)
{
    DEVICE_MANAGER.addDevice(type, id);
    return true;
}
