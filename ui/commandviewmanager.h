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
    
    QPointF getDevicePos(int id) const;

    bool addItem(std::shared_ptr<Command> cmnd);
    bool addConnection(std::shared_ptr<Command> cmnd);
    bool removeItem(std::shared_ptr<Command> cmnd);
    bool removeConnection(std::shared_ptr<Command> cmnd);
    bool changeVariables(std::shared_ptr<Command> cmnd);

    ConstructorItem *findItemWithId(int id) const;
    
private:
    QList<ConstructorItem *> _devices;
    QGraphicsScene  *_scene;
    
};

#endif // COMMANDVIEWMANAGER_H
