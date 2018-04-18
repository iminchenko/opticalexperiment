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
    Command() = default;
    Command(TypeCommand typeCommand, QPointF pos, int typeItemId, int id);
    
    TypeCommand typeCommand() const;
    QPointF pos() const;
    int typeItemId() const;
    int id() const;

    int getSourceId() const;
    int getDestId() const;
    int getSourceNum() const;
    int getDestNum() const;

    void setSourceId(int id);
    void setDestId(int id);

    void setSourceOutNum(int num);
    void setDestInNum(int num);

private:
    // общее
    TypeCommand _typeCommand;

    // для вектора
    QPointF _pos;
    int _typeItemId;
    int _id;

    // для ребра
    int _sourceId;
    int _destId;
    int _sourceNum;
    int _destNum;
};

#endif // COMAND_H
