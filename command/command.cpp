#include "command.h"

Command::Command(QPointF pos, int typeItemId)
    :typeCommand(CMND_ADD_DEVICE) {
    data.ad.x = pos.x();
    data.ad.y = pos.y();
    data.ad.typeItemId = typeItemId;
}

Command::Command(int sourceId, int destId, int sourceNum, int destNum)
    :typeCommand(CMND_ADD_CONNECTION) {
    data.ac.sourceId = sourceId;
    data.ac.destId = destId;
    data.ac.sourceNum = sourceNum;
    data.ac.destNum = destNum;
}

QPointF Command::Data::AddDevice::pos() const {
    return QPointF(x, y);
}
