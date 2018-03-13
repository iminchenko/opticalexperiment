#include "devicemanager.h"

void DeviceManager::addDevice(int type) {
    matrConn_.resize(matrConn_.getRows() + 1);
    _types.push_back(type);
}

void DeviceManager::addConnection(int sourceIdDevice,
                                  int finalDivece, int destIdDevice) {
    matrConn_[sourceIdDevice][finalDivece] = destIdDevice;
}

void DeviceManager::removeDevice(int idDevice) {
    for (int i = 0; i < matrConn_.getRows(); i++)
        matrConn_[idDevice][i] = 0;

    matrConn_.removeAt(idDevice);
}

size_t DeviceManager::deviceCount() const {
    return matrConn_.getRows();
}

