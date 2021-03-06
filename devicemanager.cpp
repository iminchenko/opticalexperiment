#include "devicemanager.h"
#include "deviceconfigs/devicedefines.h"
#include "deviceconfigs/diffractiongrating.h"
#include "deviceconfigs/display.h"
#include "deviceconfigs/laser.h"

using std::make_shared;
using std::make_unique;
using std::shared_ptr;

DeviceManager::DeviceManager() : Singleton<DeviceManager>(*this) {}

void DeviceManager::addDevice(int type, int id) {
    switch (type) {
    case deviceType::TYPE_LASER:
        _devices.push_back(make_unique<Laser>(id));
        break;
    case deviceType::TYPE_SHIELD:
        _devices.push_back(make_unique<Display>(id));
        break;
    case deviceType::TYPE_DIFFRACTION_GRATING:
        _devices.push_back(make_unique<DiffractionGrating>(id));
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
    getDeviceById(destDevId)->setConnection(
        destInput, getDeviceById(sourceDevId), sourceOut);
}

void DeviceManager::removeDevice(int idDevice) {
    auto iter = getDeviceIterById(idDevice);

    if (iter != _devices.end()) {
        _devices.erase(iter);
    }
}

void DeviceManager::removeConnection(int sourceDevId,
                                     int sourceOut,
                                     int destDevId,
                                     int destInput) {
    Q_UNUSED(sourceDevId);

    getDeviceById(destDevId)->setConnection(
        destInput, make_shared<Device>(), sourceOut);
}

void DeviceManager::changeVariables(int id, VarList vars) {
    if (id == -1) {
        return;
    }
    getDeviceById(id)->setVariables(vars);
}

std::shared_ptr<Device> DeviceManager::getDeviceById(int id) {
    auto iter = getDeviceIterById(id);

    return iter != _devices.end() ? *iter : nullptr;
}

std::vector<std::shared_ptr<Device>>::iterator
    DeviceManager::getDeviceIterById(int id) {
    return std::find_if(_devices.begin(), _devices.end(), [id](auto device) {
        return device->getId() == id;
    });
}
