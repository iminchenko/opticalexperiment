#include "device.h"
#include "deviceconfiglist.h"

Device::Device(int type, int id) :_type(type), _id(id),
    _connections(DEVICECONFIG_LIST[type].getInputCount(), {nullptr, 0, 0}),
    _waveCache(DEVICECONFIG_LIST[type].getOutputCount()) {}

Wave Device::getWave(int output) const {
    if (changed()) {
        _changed = false;

        Waves inputWaves;

        for (const auto& connection : _connections) {
            if (!connection.device.expired()) {
                inputWaves.push_back(connection.device.lock()->getWave(connection.output));
            }
            else {
                inputWaves.emplace_back(Wave());
            }
        }

        _waveCache = (DEVICECONFIG_LIST[_type].getMatrix() * inputWaves);
    }

    return _waveCache[output];
}

void Device::setConnection(int input, std::shared_ptr<Device> &source,
                           double distance, int output) {
    _changed = true;
    _connections[input] = connection(source, distance, output);
}

bool Device::changed() const {
    if (_changed)
        return true;

    for (const auto &i : _connections) {
        if (!i.device.expired() && i.device.lock()->changed()) {
            _changed = true;
            return true;
        }
    }

    return false;
}
