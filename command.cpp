#include "command.h"

Command::Command(TypeCommand typeCommand, QPointF pos, int typeItem, int typeItemId, int id)
    : _typeCommand(typeCommand), 
      _pos(pos),
      _typeItem(typeItem),
      _typeItemId(typeItemId),
      _id(id) {
    
}

TypeCommand Command::typeCommand() const {
    return _typeCommand;
}

QPointF Command::pos() const {
    return _pos;
}

int Command::typeItem() const {
    return _typeItem;
}

int Command::typeItemId() const {
    return _typeItemId;
}

int Command::id() const {
    return _id;
}
