#include "devicelist.h"

void DeviceList::loadDevices(std::string filename) {

}

const Device &DeviceList::operator[](size_t id) const {
    return _devList[id];
}

DeviceList::DeviceList() {

}
