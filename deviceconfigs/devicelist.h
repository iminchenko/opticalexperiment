#pragma once

#include <utility/singleton.hpp>
#include <vector>
#include "device.h"

#define DEVICE_LIST singleton<DeviceList>()

class DeviceList {
    friend DeviceList& singleton<DeviceList>();
public:
    void loadDevices(std::string filename);
    size_t count() const;

    const Device& operator[](size_t id) const;

protected:
    DeviceList();

private:
    std::vector<Device> _devList;
};
