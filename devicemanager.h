#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H
#include "matrix.h"

class DeviceManager
{
public:
    DeviceManager();

    void addDevice();
    void addConnection(int sourceIdDevice, int finalDivece, int destIdDevice);
    void removeDevice(int idDevice);

private:
    Matrix<int> matrConn_;


};

#endif // DEVICEMANAGER_H
