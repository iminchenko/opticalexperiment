#ifndef COMANDHANDLERVIEW_H
#define COMANDHANDLERVIEW_H
#include <QGraphicsScene>
#include <QPointF>
#include <memory>

#include "commandhandler.h"
#include "utility/singleton.hpp"
#include "deviceconfigs/devicedefines.h"
#include "command.h"


#define CH_VIEW singleton<CommandHandlerView>()

class CommandHandlerView : CommandHandler {
    friend CommandHandlerView& singleton<CommandHandlerView>();
    
public: 
    void setScene(QGraphicsScene *scene);    
    bool handler(std::shared_ptr<Command> cmnd) override;

private:
    bool addItem(std::shared_ptr<Command> cmnd);

private:
    CommandHandlerView();
    QGraphicsScene  *_scene; 
};

#endif // COMANDHANDLERVIEW_H
