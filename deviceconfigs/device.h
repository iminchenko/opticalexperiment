#pragma once

#include <complex>
#include <string>
#include <vector>
#include <memory>

#include "matrix.h"
#include "wave.h"

using std::size_t;

class Device {
    struct connection {
        std::weak_ptr<const Device> device;
        double distance;
        int output;

        connection(const std::shared_ptr<Device> &dev, double dist, int out):
        device(dev), distance(dist), output(out) {}
    };

public:
    Device() = default;
    Device(int type, int id);
    virtual ~Device() = default;

    // получить волну с заданного выхода
    virtual Wave getWave(int output) const;

    void setConnection(int input, std::shared_ptr<Device> &source,
                       double distance, int output);

protected:
    // был ли путь изменен => надо ли пересчитывать кэш
    bool changed() const;

    // графовая инфа
    // double - расстояние
    // int - id выхода
    std::vector<connection> _connections;

private:
    // базовая инфа об устройстве
    int _type = 0;
    int _id = -1;

    // для кэширования
    mutable bool _changed = true;
    mutable std::vector<Wave> _waveCache;
};
