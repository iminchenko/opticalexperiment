#ifndef COMAND_H
#define COMAND_H

#include <QPointF>
#include <QGraphicsScene>

enum TypeCommand {
    CMND_ADD_DEVICE = 0,
    CMND_ADD_CONNECTION,
    CMND_DELETE_DEVICE,
    CMND_DELETE_CONNECTION,
    CMND_REFRESH_DEVICE        // ToDo: Сделать обновление элемента
};

struct Command {
public:
    Command(QPointF pos, int typeItemId);
    Command(int sourceId, int destId, int sourceNum, int destNum);

    // общее
    TypeCommand typeCommand;

    union Data {
    struct AddDevice {
        QPointF pos() const;
        // для вертекса
        double x;
        double y;
        int typeItemId;
        int id;
    } ad;
    struct AddConnection {
        int sourceId;
        int destId;
        int sourceNum;
        int destNum;
    } ac;
    };

    Data data;
};

#endif // COMAND_H
