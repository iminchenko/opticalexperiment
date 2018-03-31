#include "device.h"
#include "deviceconfiglist.h"

using  std::vector;

Device::Device(int type, int id) :_type(type), _id(id),
    _connections(DEVICECONFIG_LIST[type].getInputCount(), {nullptr, 0, 0}),
    _waveCache(DEVICECONFIG_LIST[type].getOutputCount()) {}

// TODO: очень большая функция, разнести по подфункциям
Waves Device::getWave(int output) const {
    if (changed()) {
        _changed = false;

        for (auto &waves : _waveCache) {
            waves.clear();
        }

        vector<Waves> inputWaves;

        for (const auto& connection : _connections) {
            if (!connection.device.expired()) {
                inputWaves.push_back(connection.device.lock()->getWave(connection.output));
            } else {
                inputWaves.emplace_back();
            }
        }

        vector<Waves::const_iterator> iters;

        for (const auto &waves : inputWaves) {
            iters.push_back(waves.cbegin());
        }

        bool pushed = true;
        while (pushed) {
            pushed = false;
            // одна "порция" волн
            Waves portion;

            for (int i = 0; i < inputWaves.size(); ++i) {
                if (iters[i] != inputWaves[i].cend()) {
                    portion.push_back(*iters[i]);
                    ++iters[i];
                    pushed = true;
                } else {
                    portion.emplace_back();
                }
            }

            auto outWaves = (DEVICECONFIG_LIST[_type].getMatrix() * portion);

            size_t ratio = outWaves.size() / _waveCache.size();
            for (int i = 0; i < _waveCache.size(); ++i) {
                for (int j = 0; j < ratio; ++j) {
                    _waveCache[i].push_back(outWaves[ratio * i + j]);
                }
            }
        }
    }

    for (auto &waves : _waveCache) {
        clearEmpty(waves);
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
