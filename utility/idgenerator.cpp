#include "idgenerator.h"

IdGenerator::IdGenerator()
    : lastId(-1)
{}

IdGenerator::IdGenerator(int startValue)
    : lastId(startValue)
{}

int IdGenerator::getId() {
    return ++lastId;
}

int IdGenerator::getLastId() {
    return lastId;
}

void IdGenerator::setLastId(int id) {
    lastId = id;
}
