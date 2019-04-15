#include "commandviewmanager.h"

CommandViewManager::CommandViewManager() : Singleton<CommandViewManager> (*this) { }

QPointF CommandViewManager::getDevicePos(int id) {
    auto item = findItemWithId(id);

    if (item == nullptr) {
        throw std::logic_error("can't find device with id");
    }

    return item->pos();
}

void CommandViewManager::setScene(QGraphicsScene *scene) {
    Q_ASSERT(_scene != nullptr);
    _scene = scene;
}

bool CommandViewManager::addItemToScene(QGraphicsItem * const item) {
    if (item == nullptr)
        return false;
    
    _scene->addItem(item);
    return true;
}

bool CommandViewManager::addDevice(ConstructorItem *dev) {
    if (dev == nullptr)
        return false;
    
    _devices.push_back(dev);
    return true;
}

bool CommandViewManager::removeDevice(int id) {
    auto iter = std::find_if(_devices.begin(), _devices.end(),
                          [id](ConstructorItem *item){ return item->getId() == id; });
    
    if (iter == _devices.end())
        return false;
    
    delete *iter;
    _devices.erase(iter);
    
    return true;
}

ConstructorItem *CommandViewManager::findItemWithId(int id) {
    auto iter = std::find_if(_devices.begin(), _devices.end(),
                          [id](ConstructorItem *item){ return item->getId() == id; });

    return iter != _devices.end() ? *iter : nullptr;
}
