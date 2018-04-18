#ifndef COMAND_H
#define COMAND_H

#include <QPointF>
#include <QGraphicsScene>

enum TypeCommand {
    CMND_ADD = 0,
    CMND_DELETE,
    CMND_REFRESH        // ToDo: Сделать обновление элемента    
};

class Command {
public:
    Command(TypeCommand typeCommand, QPointF pos, int typeItemId, int id);
    
    TypeCommand typeCommand() const;
    QPointF pos() const;
    int typeItemId() const;
    int id() const; 

private:
    TypeCommand _typeCommand;
    QPointF _pos;
    int _typeItemId;
    int _id;
};

#endif // COMAND_H
