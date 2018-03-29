#include "device.h"
#include "deviceconfiglist.h"

Device::Device(int type, int id) :_type(type), _id(id),
    _connections(DEVICECONFIG_LIST[type].getInputCount(), {nullptr, 0, 0}),
    _waveCache(DEVICECONFIG_LIST[type].getOutputCount()){}

Device::~Device() {}

Wave Device::getWave(int output) const {
    if (changed()) {
        _changed = false;

        for (auto &wave : _waveCache) {
            wave = Wave();
            for (const auto &connection : _connections) {
                if (connection.device) {
                    // по-хорошему должно быть +=
                    wave = DEVICECONFIG_LIST[_type].getMatrix() *
                           connection.device->getWave(connection.output);
                }
            }
        }
    }

    return _waveCache[output];
}

void Device::setConnection(int input, const Device *source,
                           double distance, int output) {
    _connections[input] = connection(source, distance, output);
}

bool Device::changed() const {
    if (_changed)
        return true;

    for (const auto &i : _connections) {
        if (i.device && i.device->changed()) {
            _changed = true;
            return true;
        }
    }

    return false;
}
