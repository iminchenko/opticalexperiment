#include "devicemanager.h"
#include "deviceconfigs/devicedefines.h"
#include "deviceconfigs/laser.h"
#include "deviceconfigs/display.h"

using std::shared_ptr;
using std::make_unique;
using std::make_shared;

void DeviceManager::addDevice(int type, int id) {
    switch(type) {
        case deviceType::TYPE_LASER:
            _devices.push_back(make_unique<Laser>(id));
            break;
        case deviceType::TYPE_SHIELD:
            _devices.push_back(make_unique<Display>(id));
            break;
        default:
            _devices.push_back(make_unique<Device>(type, id));
            break;
    }
}

void DeviceManager::addConnection(int sourceDevId,
                                  int sourceOut,
                                  int destDevId,
                                  int destInput) {
    // 10 - magic number for distance
    getDeviceById(destDevId)->setConnection(
                destInput,getDeviceById(sourceDevId),sourceOut);
}

void DeviceManager::removeDevice(int idDevice) {
    auto iter = _devices.begin();
    while (iter != _devices.end() && iter->get()->getId() != idDevice) {
         iter++;
    }
    if(iter != _devices.end()) {
        iter->reset();
        _devices.erase(iter);
    }
}

void DeviceManager::removeConnection(int sourceDevId,
                                     int sourceOut,
                                     int destDevId,
                                     int destInput) {
    Q_UNUSED(sourceDevId);
    /* ToDo: С этой функцией что-то не так? Или я наркоман? Как минимум,
       тут нужно что-то сделать с sourceDevId */
    getDeviceById(destDevId)->setConnection(destInput, make_shared<Device>(),
                                       sourceOut);
}

void DeviceManager::changeVariables(int id, VarList vars) {
    getDeviceById(id)->setVariables(vars);
}

size_t DeviceManager::deviceCount() const {
    return (size_t)_matrConn.rows();
}

int DeviceManager::getMaxId() const {
    return _maxId;
}

DeviceManager::DeviceManager() :_maxId(0) {}

Display *DeviceManager::getDisplay() const {
    for (auto &i : _devices){
        if (i.get()->getType() == deviceType::TYPE_SHIELD) {
            auto disp = dynamic_cast<Display*>(i.get());
            if (disp)
                return disp;
        }
    }
    return nullptr;
}

std::shared_ptr<Device> DeviceManager::getDeviceById(int id) {
    for (auto i : _devices){
        if (i.get()->getId() == id) {
             return i;
        }
    }
    return nullptr;
}
