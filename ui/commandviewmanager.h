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
    QGraphicsScene * scene() const;
    
    QList<ConstructorItem *> &devices();
    
    QPointF getDevicePos(int id) const;
    ConstructorItem *findItemWithId(int id) const;
    
private:
    QList<ConstructorItem *> _devices;
    QGraphicsScene  *_scene;
    
};

#endif // COMMANDVIEWMANAGER_H
