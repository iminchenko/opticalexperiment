#pragma once

#include <vector>

#include "singleton.hpp"
#include "deviceviewconfig.h"

#define DEVICEVIEW_LIST singleton<DeviceViewList>()

class DeviceViewList {
    friend DeviceViewList& singleton<DeviceViewList>();
public:
    void loadDevices(std::string filename);

    const DeviceViewConfig &operator[](size_t id) const;

protected:
    DeviceViewList();

private:
    std::vector<DeviceViewConfig> _devList;
};
