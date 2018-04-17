#ifndef COMAND_H
#define COMAND_H

#include <QPointF>
#include <QGraphicsScene>

enum TypeCommand {
    ADD = 0,
    DELETE,
    REFRESH,        // ToDo: Сделать обновление элемента    
};

class Command
{
public:
    Command(TypeCommand typeCommand, QPointF pos, int typeItem, 
            int typeItemId, int id);
    
    TypeCommand typeCommand() const;
    QPointF pos() const;
    int typeItem() const;
    int typeItemId() const;
    int id() const; 

private:
    TypeCommand _typeCommand;
    QPointF _pos;
    int _typeItem;
    int _typeItemId;
    int _id;
};

#endif // COMAND_H
