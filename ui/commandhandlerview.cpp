#include "commandhandlerview.h"
#include "laseritem.h"
#include "shielditem.h"
#include "genericitem.h"

CommandHandlerView::CommandHandlerView() {
    
}

void CommandHandlerView::setScene(QGraphicsScene *scene) {
    _scene = scene;
}

bool CommandHandlerView::addItem(std::shared_ptr<Command> cmnd) {
    
    if (cmnd->data.ad.typeItemId == deviceType::TYPE_LASER) {
        _scene->addItem(new LaserItem(cmnd->data.ad.pos(), cmnd->data.ad.id));
    } else if (cmnd->data.ad.typeItemId == deviceType::TYPE_SHIELD) {
        _scene->addItem(new ShieldItem(cmnd->data.ad.pos(), cmnd->data.ad.id));
    } else {
        _scene->addItem(new GenericItem(cmnd->data.ad.pos(), cmnd->data.ad.id,
                                        cmnd->data.ad.typeItemId));
    }
    
    return true;
}

//ToDo: В полне возможно, что нужно перенсти этот метод в CommandHandler
bool CommandHandlerView::handle(std::shared_ptr<Command> cmnd) {
    switch (cmnd->typeCommand) {
    case TypeCommand::CMND_ADD_DEVICE:
        return addItem(cmnd);
    case TypeCommand::CMND_DELETE_DEVICE:
        return false;
    case TypeCommand::CMND_REFRESH_DEVICE:
        return false; 
    default:
        return true;
    }
}
