#pragma once

#include <complex>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <utility>

#include "matrix.h"
#include "wave.h"

using std::size_t;

class Device {
    struct connection {
        std::weak_ptr<const Device> device;
        int output;

        connection() = default;
        connection(std::shared_ptr<Device> dev, int out):
        device(dev), output(out) {}
    };

public:
    Device();
    Device(int type, int id);
    virtual ~Device() = default;

    // получить волну с заданного выхода
    virtual Waves getWave(int output = 0) const;

    void setConnection(int input, std::shared_ptr<Device> source, int output);
    connection getConnection(int input);

    virtual void setVariables(VarList vars);
    virtual VarList getVariables() const;

    int getType() const;
    int getId() const;
protected:
    // был ли путь изменен => надо ли пересчитывать кэш
    bool changed() const;

    std::vector<connection> _connections;

    mutable bool _changed = true;

private:
    void updateWaveChache() const;
    void updateMatrix();

private:
    // базовая инфа об устройстве
    int _type = 0;
    int _id = -1;

    TransMatrix _concreteMatrix;
    VarList _concreteVariables;

    // для кэширования
    mutable std::vector<Waves> _waveCache;
    mutable std::vector<bool> _connectionCache;
};
