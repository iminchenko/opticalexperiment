#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include "utility/singleton.hpp"

#define DEVICE_ID_GEN singleton<IdGenerator>()

class IdGenerator {
public:
    IdGenerator();
    IdGenerator(int startValue);

    int getId();
    int getLastId();

    void setLastId(int id);

private:
    int lastId;
};

#endif // IDGENERATOR_H
