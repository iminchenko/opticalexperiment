#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include "utility/singleton.hpp"

class IdGenerator : public Singleton<IdGenerator> {
public:
    IdGenerator(int startValue = 0);

    int getId();
    int getLastId();

    void setLastId(int id);

private:
    int lastId;
};

#endif // IDGENERATOR_H
