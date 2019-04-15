#include "commandviewmanager.h"

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

QGraphicsScene *CommandViewManager::scene() const
{
    return _scene;
}

QList<ConstructorItem *>& CommandViewManager::devices()
{
    return _devices;
}

ConstructorItem *CommandViewManager::findItemWithId(int id) const {
    auto iter = std::find_if(_devices.begin(), _devices.end(),
                          [id](ConstructorItem *item){ return item->getId() == id; });

    return iter != _devices.end() ? *iter : nullptr;
}
