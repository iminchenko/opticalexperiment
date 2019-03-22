#ifndef COMAND_H
#define COMAND_H

#include <QPointF>
#include <QGraphicsScene>
#include <memory>

#include "deviceconfigs/devicedefines.h"

enum TypeCommand {
    CMND_ADD_DEVICE = 0,
    CMND_ADD_CONNECTION,
    CMND_DELETE_DEVICE,
    CMND_DELETE_CONNECTION,
    CMND_CHANGE_VARIABLE,
    CMND_REFRESH_DEVICE      
};

class Command;

using pCommand = std::shared_ptr<Command>;

class Command {
public:
    static pCommand AddDevice(QPointF pos, int typeItemId, int id = 0);
    static pCommand AddConnection(int sourceId, int destId,
                                  int sourceNum, int destNum);
    static pCommand DeleteDevice(int id);
    static pCommand DeleteConnection(int sourceId, int destId,
                                     int sourceNum, int destNum);
    static pCommand ChangeValues(int id, VarList values);

    // общее
    TypeCommand typeCommand;

    VarList  varList;

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
    struct DeleteDevice {
        int id;
    } dd;
    struct DeleteConnection {
        int sourceId;
        int destId;
        int sourceNum;
        int destNum;
    } dc;
    struct ChangeValue {
        int id;
    } cv;
    } data;
};

#endif // COMAND_H
