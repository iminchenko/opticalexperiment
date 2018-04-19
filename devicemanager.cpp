#include "devicemanager.h"
#include "deviceconfigs/devicedefines.h"
#include "deviceconfigs/laser.h"
#include "deviceconfigs/display.h"

using std::shared_ptr;
using std::make_unique;
using std::make_shared;

void DeviceManager::addDevice(int type, int id) {
//    _matrConn.resize(_matrConn.getRows() + 1);
//    _types.push_back(type);
    if (_devices.size() <= id) {
        ++_maxId;
        _devices.resize(_maxId);
    }

    switch(type) {
        case deviceType::TYPE_LASER:
            _devices[id] = make_unique<Laser>(id);
            break;
        case deviceType::TYPE_SHIELD:
            _devices[id] = make_unique<Display>(id);
            break;
        default:
            _devices[id] = make_unique<Device>(type, id);
            break;
    }
}

void DeviceManager::addConnection(int sourceDevId, int sourceOut,
                                  int destDevId, int destInput) {
    // 10 - magic number for distance
    _devices[destDevId]->setConnection(destInput, _devices[sourceDevId],
                                       sourceOut);
}

void DeviceManager::removeDevice(int idDevice) {
    _devices[idDevice].reset();
}

void DeviceManager::removeConnection(int sourceDevId, int sourceOut,
                                     int destDevId, int destInput) {
    _devices[destDevId]->setConnection(destInput, make_shared<Device>(),
                                       sourceOut);
}

void DeviceManager::changeVariables(int id, VarList vars) {
    _devices[id]->setVariables(vars);
}

size_t DeviceManager::deviceCount() const {
    return (size_t)_matrConn.getRows();
}

int DeviceManager::getMaxId() const {
    return _maxId;
}

DeviceManager::DeviceManager() :_maxId(0) {}

Display *DeviceManager::getDisplay() const {
    for (auto &i : _devices){
        auto disp = dynamic_cast<Display*>(i.get());
        if (disp)
            return disp;
    }

    return nullptr;
}
