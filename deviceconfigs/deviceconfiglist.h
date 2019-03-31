#pragma once

#include <vector>

#include "utility/singleton.hpp"
#include "deviceconfig.h"

class DeviceConfigList : public Singleton<DeviceConfigList> {
public:
    DeviceConfigList();
    
    void loadDevices(std::string filename);
    size_t count() const;

    const DeviceConfig &operator[](int id) const;

private:
    std::vector<DeviceConfig> _devList;
    DeviceConfig _dummyDevice;
};
