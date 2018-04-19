#ifndef COMAND_H
#define COMAND_H

#include <QPointF>
#include <QGraphicsScene>
#include <memory>

enum TypeCommand {
    CMND_ADD_DEVICE = 0,
    CMND_ADD_CONNECTION,
    CMND_DELETE_DEVICE,
    CMND_DELETE_CONNECTION,
    CMND_REFRESH_DEVICE        // ToDo: Сделать обновление элемента
};

struct Command {
public:
    static std::shared_ptr<Command> AddDevice(QPointF pos, int typeItemId, int id = 0);
    static std::shared_ptr<Command> AddConnection(int sourceId, int destId,
                                 int sourceNum, int destNum);
    static std::shared_ptr<Command> DeleteDevice(int id);
    static std::shared_ptr<Command> DeleteConnection(int sourceId, int destId,
                                    int sourceNum, int destNum);

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
    struct DeleteDevice {
        int id;
    } dd;
    struct DeleteConnection {
        int sourceId;
        int destId;
        int sourceNum;
        int destNum;
    } dc;
    };

    Data data;
};

#endif // COMAND_H
