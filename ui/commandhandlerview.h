#ifndef COMANDHANDLERVIEW_H
#define COMANDHANDLERVIEW_H
#include <memory>
#include <QGraphicsScene>
#include <QPointF>
#include <QList>

#include "command/commandhandler.h"
#include "utility/singleton.hpp"
#include "deviceconfigs/devicedefines.h"
#include "command/command.h"

class ConstructorItem;

class CommandHandlerView :CommandHandler,  public Singleton<CommandHandlerView> {
public: 
    CommandHandlerView();    
    
    void setScene(QGraphicsScene *scene);    
    bool handle(std::shared_ptr<Command> cmnd) override;

    // FIXME: этот метод должен быть в другом классе
    QPointF getDevicePos(int id);

private:
    bool addItem(std::shared_ptr<Command> cmnd);
    bool addConnection(std::shared_ptr<Command> cmnd);
    bool removeItem(std::shared_ptr<Command> cmnd);
    bool removeConnection(std::shared_ptr<Command> cmnd);
    bool changeVariables(std::shared_ptr<Command> cmnd);

    ConstructorItem *findItemWithId(int id);

private:
    QGraphicsScene  *_scene;

    QList<ConstructorItem *> _devices;
};

#endif // COMANDHANDLERVIEW_H
