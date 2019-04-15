#ifndef COMMANDVIEWMANAGER_H
#define COMMANDVIEWMANAGER_H
#include <QPointF>

#include "constructoritem.h"
#include "utility/singleton.hpp"
#include "command/command.h"

class CommandViewManager : public Singleton<CommandViewManager>
{
public:
    CommandViewManager();
        
    void setScene(QGraphicsScene *scene);
    bool addItemToScene(QGraphicsItem * const item);
    
    bool addDevice(ConstructorItem *dev);
    bool removeDevice(int id);
    
    QPointF getDevicePos(int id);
    ConstructorItem * findItemWithId(int id);
    
private:
    QList<ConstructorItem *> _devices;
    QGraphicsScene  *_scene;
};

#endif // COMMANDVIEWMANAGER_H
