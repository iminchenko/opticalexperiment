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
    
    bool handle(std::shared_ptr<Command> cmnd) override;
};

#endif // COMANDHANDLERVIEW_H
