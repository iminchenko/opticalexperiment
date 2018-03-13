#pragma once
#include <vector>

#include "matrix.h"
#include "utility/singleton.hpp"

#define DEVICE_MANAGER singleton<DeviceManager>()

class DeviceManager {
public:
    friend DeviceManager& singleton<DeviceManager>();

    enum deviceType {
        TYPE_LASER = -1,
        TYPE_SHIELD = -2
    };

    void addDevice(int type);
    void addConnection(int sourceIdDevice, int finalDivece, int destIdDevice);
    // в разработке
    void removeDevice(int idDevice);

    size_t deviceCount() const;

protected:
    DeviceManager() = default;

private:
    Matrix<int> matrConn_;
    std::vector<int> _types;
};
