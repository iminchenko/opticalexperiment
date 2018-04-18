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
    
    if (cmnd->typeItemId() == deviceType::TYPE_LASER) {
        _scene->addItem(new LaserItem(cmnd->pos(), cmnd->id()));
    } else if (cmnd->typeItemId() == deviceType::TYPE_SHIELD) {
        _scene->addItem(new ShieldItem(cmnd->pos(), cmnd->id()));
    } else {
        _scene->addItem(new GenericItem(cmnd->pos(), cmnd->id(),
                                        cmnd->typeItemId()));
    }
    
    return true;
}

//ToDo: В полне возможно, что нужно перенсти этот метод в CommandHandler
bool CommandHandlerView::handler(std::shared_ptr<Command> cmnd) {
    switch (cmnd->typeCommand()) {
    case TypeCommand::CMND_ADD: 
        return addItem(cmnd);
    case TypeCommand::CMND_DELETE:
        return false;
    case TypeCommand::CMND_REFRESH:
        return false; 
    default:
        return true;
    }
}
