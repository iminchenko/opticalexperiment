#include "commandhandlerview.h"
#include "laseritem.h"
#include "shielditem.h"
#include "genericitem.h"
#include "connectionitem.h"

CommandHandlerView::CommandHandlerView() {
    
}

void CommandHandlerView::setScene(QGraphicsScene *scene) {
    _scene = scene;
}

//ToDo: Вполне возможно, что нужно перенсти этот метод в CommandHandler
bool CommandHandlerView::handle(std::shared_ptr<Command> cmnd) {
    switch (cmnd->typeCommand) {
    case TypeCommand::CMND_ADD_DEVICE:
        return addItem(cmnd);
        case TypeCommand::CMND_ADD_CONNECTION:
            return addConnection(cmnd);
    case TypeCommand::CMND_DELETE_DEVICE:
            return removeItem(cmnd);
    case TypeCommand::CMND_REFRESH_DEVICE:
        return false; 
    default:
        return true;
    }
}

bool CommandHandlerView::addItem(std::shared_ptr<Command> cmnd) {
    ConstructorItem *newDevice = nullptr;

    if (cmnd->data.ad.typeItemId == deviceType::TYPE_LASER) {
        newDevice = new LaserItem(cmnd->data.ad.pos(), cmnd->data.ad.id);
    } else if (cmnd->data.ad.typeItemId == deviceType::TYPE_SHIELD) {
        newDevice = new ShieldItem(cmnd->data.ad.pos(), cmnd->data.ad.id);
    } else {
        newDevice = new GenericItem(cmnd->data.ad.pos(), cmnd->data.ad.id,
                                    cmnd->data.ad.typeItemId);
    }

    _scene->addItem(newDevice);

    _devices.push_back(newDevice);

    return true;
}

bool CommandHandlerView::addConnection(std::shared_ptr<Command> cmnd) {
    auto source = findItemWithId(cmnd->data.ac.sourceId);
    auto dest = findItemWithId(cmnd->data.ac.destId);

    if (!source || !dest)
        return false;

    auto v1 = source->getOutput(cmnd->data.ac.sourceNum);
    auto v2 = dest->getInput(cmnd->data.ac.destNum);

    _scene->addItem(new ConnectionItem(v1, v2));
}

bool CommandHandlerView::removeItem(std::shared_ptr<Command> cmnd) {
    auto iter = std::find_if(_devices.begin(), _devices.end(),
                             [cmnd](ConstructorItem *item)
                             { return item->getId() == cmnd->data.dd.id; });

    if (iter == _devices.end())
        return false;

    delete *iter;
    _devices.erase(iter);
}

ConstructorItem *CommandHandlerView::findItemWithId(int id) {
    auto iter = std::find_if(_devices.begin(), _devices.end(),
                          [id](ConstructorItem *item){ return item->getId() == id; });

    if (iter == _devices.end())
        return nullptr;

    return *iter;
}
