#include "command.h"

Command::Command(TypeCommand typeCommand, QPointF pos, int typeItemId, int id)
    : _typeCommand(typeCommand), 
      _pos(pos),
      _typeItemId(typeItemId),
      _id(id) {
    
}

TypeCommand Command::typeCommand() const {
    return _typeCommand;
}

QPointF Command::pos() const {
    return _pos;
}

int Command::typeItemId() const {
    return _typeItemId;
}

int Command::id() const {
    return _id;
}
