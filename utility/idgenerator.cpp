#include "idgenerator.h"

IdGenerator::IdGenerator(int startValue)
    : Singleton<IdGenerator>(*this),
      lastId(startValue) {}

int IdGenerator::getId() {
    return lastId++;
}

int IdGenerator::getLastId() {
    return lastId;
}
