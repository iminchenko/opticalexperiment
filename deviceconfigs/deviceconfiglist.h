#pragma once

#include <vector>

#include "utility/singleton.hpp"
#include "deviceconfig.h"

#define DEVICECONFIG_LIST singleton<DeviceConfigList>()

class DeviceConfigList {
    friend DeviceConfigList& singleton<DeviceConfigList>();
public:
    void loadDevices(std::string filename);
    size_t count() const;

    const DeviceConfig &operator[](int id) const;

protected:
    DeviceConfigList() = default;

private:
    std::vector<DeviceConfig> _devList;
    DeviceConfig _dummyDevice;
};
