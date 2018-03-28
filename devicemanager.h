#pragma once

#include <vector>

#include "matrix.h"
#include "utility/singleton.hpp"
#include "deviceconfigs/devicedefines.h"
#include "deviceconfigs/device.h"

#define DEVICE_MANAGER singleton<DeviceManager>()

class DeviceManager {
public:
    friend DeviceManager& singleton<DeviceManager>();

    void addDevice(int type, int id);
    void addConnection(int sourceIdDevice, int finalDivece, int destIdDevice);
    void addConnection(int sourceDevId, int sourceOut,
                       int destDevId, int destInput);
    // в разработке
    void removeDevice(int idDevice);

    size_t deviceCount() const;
    int getMaxId() const;

protected:
    DeviceManager();

private:
    Matrix<int> _matrConn;
    std::vector<int> _types;

    int _maxId;

    std::vector<Device> _devices;
};
