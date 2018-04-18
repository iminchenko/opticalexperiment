#ifndef COMANDHANDLERVIEW_H
#define COMANDHANDLERVIEW_H
#include <QGraphicsScene>
#include <QPointF>

#include "utility/singleton.hpp"
#include "deviceconfigs/devicedefines.h"
#include "command.h"

#define CH_VIEW singleton<CommandHandlerView>()

class CommandHandlerView
{
    friend CommandHandlerView& singleton<CommandHandlerView>();
    
public:    
    void setScene(QGraphicsScene *scene);
    bool addItem(QPointF pos, int type, int typeId, int id);
    
private:
    CommandHandlerView();
    QGraphicsScene  *_scene; 
};

#endif // COMANDHANDLERVIEW_H
