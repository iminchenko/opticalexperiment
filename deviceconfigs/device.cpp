#include "device.h"
#include "deviceconfiglist.h"

Device::Device(int type, int id) :_type(type), _id(id),
    _connections(DEVICECONFIG_LIST[type].getInputCount(), {nullptr, 0, 0}) {}

Device::~Device() {}

Wave Device::getWave(int output) const {
    if (changed()) {
        // магия с расчетом волн
//        _waveCache[output] = DEVICECONFIG_LIST[_type].getMatrix()
//                              *_waveCache[output];
        _changed = false;
    }

    return _waveCache[output];
}

void Device::setConnection(int input, const Device *source,
                           double distance, int output) {
    _connections[input] =
             make_triple<const Device *, double, int>(source, distance, output);
}

bool Device::changed() const {
    if (_changed)
        return true;

    for (const auto &i : _connections) {
        if (i.first->changed()) {
            return _changed = true;
        }
    }

    return false;
}
