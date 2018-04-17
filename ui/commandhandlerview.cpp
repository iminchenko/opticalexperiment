#include "commandhandlerview.h"
#include "laseritem.h"
#include "shielditem.h"
#include "genericitem.h"

CommandHandlerView::CommandHandlerView() {
    
}

void CommandHandlerView::setScene(QGraphicsScene *scene) {
    _scene = scene;
}

bool CommandHandlerView::addItem(QPointF pos, int type, int typeId, int id) {
    if (type == deviceType::TYPE_LASER) {
        _scene->addItem(new LaserItem(pos, id));
    } else if (type == deviceType::TYPE_SHIELD) {
        _scene->addItem(new ShieldItem(pos, id));
    } else {
        _scene->addItem(new GenericItem(pos, id, typeId));
    }
    
    return true;
}
