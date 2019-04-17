#include "device.h"
#include "deviceconfiglist.h"
#include "utility/parser.h"

using  std::vector;

Device::Device() :
    _changed(true),
    _type(0),
    _id(-1)
{}

Device::Device(int type, int id) :
    _connections(DEVICECONFIG_LIST[type].getInputCount(), {nullptr, 0}),
    _changed(true),
    _type(type), 
    _id(id),    
    _concreteMatrix(DEVICECONFIG_LIST[type].exprMatrix().rows(),
                    DEVICECONFIG_LIST[type].exprMatrix().columns()),
    _concreteVariables(DEVICECONFIG_LIST[type].getVariables()),
    _waveCache(DEVICECONFIG_LIST[type].getOutputCount()),
    _connectionCache(DEVICECONFIG_LIST[type].getInputCount(), false) {
    updateMatrix();
}

Waves Device::getWave(int output) const {
    if (changed()) {
        _changed = false;

        updateWaveChache();
    }

    for (auto &waves : _waveCache) {
        clearEmpty(waves);
    }

    return _waveCache[output];
}

void Device::setConnection(int input,
                           std::shared_ptr<Device> source,
                           int output) {
    _changed = true;
    _connections[input] = connection(source, output);
}

void Device::setVariables(VarList vars) {
    _concreteVariables = std::move(vars);

    _changed = true;

    updateMatrix();
}

bool Device::changed() const {
    if (_changed)
        return true;

    int j = 0;
    for (const auto &i : _connections) {
        if ((!i.device.expired() && i.device.lock()->changed())
            || (i.device.expired() && _connectionCache[j++])) {
            _changed = true;
            return true;
        }
    }

    return false;
}

void Device::updateWaveChache() const {
    for (auto &waves : _waveCache) {
        waves.clear();
    }

    vector<Waves> inputWaves;

    int j = 0;
    for (const auto& connection : _connections) {
        if (!connection.device.expired()) {
            _connectionCache[j++] = true;
            inputWaves.push_back(connection.device.lock()->getWave(connection.output));
        } else {
            _connectionCache[j++] = false;
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

        for (std::size_t i = 0; i < inputWaves.size(); ++i) {
            if (iters[i] != inputWaves[i].cend()) {
                portion.push_back(*iters[i]);
                ++iters[i];
                pushed = true;
            } else {
                portion.emplace_back();
            }
        }

        auto outWaves = (_concreteMatrix * portion);

        size_t ratio = outWaves.size() / _waveCache.size();
        for (size_t i = 0; i < _waveCache.size(); ++i) {
            for (size_t j = 0; j < ratio; ++j) {
                _waveCache[i].push_back(outWaves[ratio * i + j]);
            }
        }
    }
}

void Device::updateMatrix() {
    for (size_t i = 0; i < _concreteMatrix.rows(); ++i) {
        for (size_t j = 0; j < _concreteMatrix.columns(); ++j) {
            _concreteMatrix[i][j].real(
                evaluateExprassion(DEVICECONFIG_LIST[_type].exprMatrix()[i][j].first,
                                   _concreteVariables)
            );
            _concreteMatrix[i][j].imag(
                evaluateExprassion(DEVICECONFIG_LIST[_type].exprMatrix()[i][j].second,
                                   _concreteVariables)
            );
        }
    }
}

int Device::getType() const {
    return _type;
}

int Device::getId() const {
    return _id;
}

void Device::setType(int type) {
    _type = type;
}
