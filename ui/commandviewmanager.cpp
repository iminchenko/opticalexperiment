#include "commandviewmanager.h"

#include "laseritem.h"
#include "shielditem.h"
#include "genericitem.h"
#include "connectionitem.h"

CommandViewManager::CommandViewManager() : Singleton<CommandViewManager> (*this) { }

QPointF CommandViewManager::getDevicePos(int id) const {
    auto item = findItemWithId(id);

    if (!item) {
        throw std::logic_error("can't find device with id");
    }

    return item->pos();
}

void CommandViewManager::setScene(QGraphicsScene *scene) {
    Q_ASSERT(_scene != nullptr);
    _scene = scene;
}

bool CommandViewManager::addItem(std::shared_ptr<Command> cmnd) {
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

bool CommandViewManager::addConnection(std::shared_ptr<Command> cmnd) {
    auto source = findItemWithId(cmnd->data.ac.sourceId);
    auto dest = findItemWithId(cmnd->data.ac.destId);

    if (!source || !dest) {
        return false;
    }

    auto v1 = source->getOutput(cmnd->data.ac.sourceNum);
    auto v2 = dest->getInput(cmnd->data.ac.destNum);

    _scene->addItem(new ConnectionItem(v1, v2));
    
    /* ToDo: Точно ли так надо возвращать?*/
    return true;
}

bool CommandViewManager::removeItem(std::shared_ptr<Command> cmnd) {
    auto iter = std::find_if(_devices.begin(), _devices.end(),
                             [cmnd](ConstructorItem *item)
                             { return item->getId() == cmnd->data.dd.id; });

    if (iter == _devices.end()) {
        return false;
    }

    delete *iter;
    _devices.erase(iter);
    
    return true;
}

bool CommandViewManager::removeConnection(std::shared_ptr<Command> cmnd) {
    auto vertex = findItemWithId(cmnd->data.dc.sourceId);

    if (!vertex) {
        // что-то поломалось
        return false;
    }

    auto out = vertex->getOutput(cmnd->data.dc.sourceNum);

    delete out->getConnection();

    return true;
}

bool CommandViewManager::changeVariables(std::shared_ptr<Command> cmnd) {
    auto device = findItemWithId(cmnd->data.cv.id);

    if (!device) {
        return false;
    }

    QMap<QString, double> properties;

    for (const auto &i: cmnd->varList) {
        properties[i.first.c_str()] = i.second;
    }

    device->setProperties(properties);

    return true;
}

ConstructorItem *CommandViewManager::findItemWithId(int id) const {
    auto iter = std::find_if(_devices.begin(), _devices.end(),
                          [id](ConstructorItem *item){ return item->getId() == id; });

    return iter != _devices.end() ? *iter : nullptr;
}
