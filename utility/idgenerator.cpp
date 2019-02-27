#include "idgenerator.h"

IdGenerator::IdGenerator()
    : lastId(0)
{}

IdGenerator::IdGenerator(int startValue)
    : lastId(startValue)
{}

int IdGenerator::getId() {
    return lastId++;
}

int IdGenerator::getLastId() {
    return lastId;
}
