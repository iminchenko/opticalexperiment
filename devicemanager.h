#pragma once

#include <vector>
#include <memory>
#include <deviceconfigs/display.h>

#include "matrix.h"
#include "utility/singleton.hpp"
#include "deviceconfigs/device.h"

#define DEVICE_MANAGER singleton<DeviceManager>()

class DeviceManager {
public:
    friend DeviceManager& singleton<DeviceManager>();

    void addDevice(int type, int id);
    void addConnection(int sourceDevId, int sourceOut,
                       int destDevId, int destInput);
    // в разработке
    void removeDevice(int idDevice);
    void removeConnection(int sourceDevId, int sourceOut,
                          int destDevId, int destInput);
    void changeVariables(int id, VarList vars);

    size_t deviceCount() const;
    int getMaxId() const;

    std::shared_ptr<Device> getDeviceById(int id);
protected:
    DeviceManager();

private:
    std::vector<std::shared_ptr<Device>>::iterator getDeviceIterById(int id);

    Matrix<int> _matrConn;
    std::vector<int> _types;

    int _maxId;

    std::vector<std::shared_ptr<Device>> _devices;
};
