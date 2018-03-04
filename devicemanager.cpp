#include "devicemanager.h"

DeviceManager::DeviceManager()
{

}

void DeviceManager::addDevice()
{
    matrConn_.resize(matrConn_.getRows() + 1);
}

void DeviceManager::addConnection(int sourceIdDevice,
                                  int finalDivece, int destIdDevice)
{
    matrConn_[sourceIdDevice][finalDivece] = destIdDevice;
}

void DeviceManager::removeDevice(int idDevice)
{
    for (int i = 0; i < matrConn_.getRows(); i++)
        matrConn_[idDevice][i] = 0;

    matrConn_.removeAt(idDivice);
}

