#include "devicemanager.h"

void DeviceManager::addDevice(int type, int id) {
//    _matrConn.resize(_matrConn.getRows() + 1);
//    _types.push_back(type);
    if (_devices.size() <= id)
        _devices.resize(id + 1);
    _devices[id] = Device(type, id);
}

void DeviceManager::addConnection(int sourceIdDevice,
                                  int finalDivece, int destIdDevice) {
    _matrConn[sourceIdDevice][finalDivece] = destIdDevice;
}

void DeviceManager::addConnection(int sourceDevId, int sourceOut,
                                  int destDevId, int destInput) {
    // 10 - magic number for distance
    _devices[destDevId].setConnection(destInput, &_devices[sourceDevId],
                                      10, sourceOut);
}

void DeviceManager::removeDevice(int idDevice) {
    for (int i = 0; i < _matrConn.getRows(); i++)
        _matrConn[idDevice][i] = 0;

    _matrConn.removeAt(idDevice);
}

size_t DeviceManager::deviceCount() const {
    return _matrConn.getRows();
}

int DeviceManager::getMaxId() const {
    return _maxId;
}

DeviceManager::DeviceManager() :_maxId(0) {

}

