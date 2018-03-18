#pragma once

#include <complex>
#include <string>
#include <vector>

#include "matrix.h"
#include "wave.h"
#include "utility/triple.hpp"

using std::size_t;

class Device {
public:
    Device() = default;
    Device(int type, int id);
    virtual ~Device();

    // получить волну с заданного выхода
    virtual Wave getWave(int output) const;

    void setConnection(int input, const Device *source, double distance,
                       int output);

protected:
    // был ли путь изменен => надо ли пересчитывать кэш
    bool changed() const;

private:
    int _type;
    int _id;

    // графовая инфа
    // double - расстояниеЫ
    // int - id выхода
    std::vector<triple<const Device *, double, int>> _connections;

    // для кэширования
    mutable bool _changed;
    mutable std::vector<Wave> _waveCache;
};
