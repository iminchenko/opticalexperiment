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

    const DeviceConfig &operator[](size_t id) const;

protected:
    DeviceConfigList();

private:
    std::vector<DeviceConfig> _devList;
};