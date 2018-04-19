#include "command.h"

QPointF Command::Data::AddDevice::pos() const {
    return QPointF(x, y);
}

std::shared_ptr<Command> Command::AddDevice(QPointF pos, int typeItemId, int id) {
    auto cmnd = std::make_shared<Command>();

    cmnd->typeCommand = CMND_ADD_DEVICE;
    cmnd->data.ad.x = pos.x();
    cmnd->data.ad.y = pos.y();
    cmnd->data.ad.typeItemId = typeItemId;
    cmnd->data.ad.id = id;

    return cmnd;
}

std::shared_ptr<Command> Command::AddConnection(int sourceId, int destId,
                                                int sourceNum, int destNum) {
    auto cmnd = std::make_shared<Command>();

    cmnd->typeCommand = CMND_ADD_CONNECTION;
    cmnd->data.ac.sourceId = sourceId;
    cmnd->data.ac.destId = destId;
    cmnd->data.ac.sourceNum = sourceNum;
    cmnd->data.ac.destNum = destNum;

    return cmnd;
}

std::shared_ptr<Command> Command::DeleteDevice(int id) {
    auto cmnd = std::make_shared<Command>();

    cmnd->typeCommand = CMND_DELETE_DEVICE;
    cmnd->data.dd.id = id;

    return cmnd;
}

std::shared_ptr<Command> Command::DeleteConnection(int sourceId, int destId,
                                                   int sourceNum, int destNum) {
    auto cmnd = AddConnection(sourceId, destId, sourceNum, destNum);

    cmnd->typeCommand = CMND_DELETE_CONNECTION;

    return cmnd;
}
