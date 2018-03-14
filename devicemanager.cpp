#include "devicemanager.h"

void DeviceManager::addDevice(int type) {
    _matrConn.resize(_matrConn.getRows() + 1);
    _types.push_back(type);
}

void DeviceManager::addConnection(int sourceIdDevice,
                                  int finalDivece, int destIdDevice) {
    _matrConn[sourceIdDevice][finalDivece] = destIdDevice;
}

void DeviceManager::removeDevice(int idDevice) {
    for (int i = 0; i < _matrConn.getRows(); i++)
        _matrConn[idDevice][i] = 0;

    _matrConn.removeAt(idDevice);
}

size_t DeviceManager::deviceCount() const {
    return _matrConn.getRows();
}

