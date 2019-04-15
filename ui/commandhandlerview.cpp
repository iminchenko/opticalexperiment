#include <stdexcept>

#include "commandhandlerview.h"
#include "laseritem.h"
#include "shielditem.h"
#include "genericitem.h"
#include "connectionitem.h"

#include "ui/commandviewmanager.h"

CommandHandlerView::CommandHandlerView() 
    : CommandHandler(nullptr),
      Singleton<CommandHandlerView>(*this) {}


//ToDo: Вполне возможно, что нужно перенести этот метод в CommandHandler
bool CommandHandlerView::handle(std::shared_ptr<Command> cmnd) {
    switch (cmnd->typeCommand) {
    case TypeCommand::CMND_ADD_DEVICE:
        return addItem(cmnd);
    case TypeCommand::CMND_ADD_CONNECTION:
        return addConnection(cmnd);
    case TypeCommand::CMND_DELETE_DEVICE:
        return removeItem(cmnd);
    case TypeCommand::CMND_DELETE_CONNECTION:
        return removeConnection(cmnd);
    case TypeCommand::CMND_CHANGE_VARIABLE:
        return changeVariables(cmnd);
    case TypeCommand::CMND_REFRESH_DEVICE:
        return true;
    default:
        return true;
    }
}

bool CommandHandlerView::addItem(std::shared_ptr<Command> cmnd) {
    ConstructorItem *newDevice = nullptr;

    if (cmnd->data.ad.typeItemId == deviceType::TYPE_LASER) {
        newDevice = new LaserItem(cmnd->data.ad.pos(), cmnd->data.ad.id);
    } else if (cmnd->data.ad.typeItemId == deviceType::TYPE_SHIELD) {
        newDevice = new ShieldItem(cmnd->data.ad.pos(), cmnd->data.ad.id);
    } else {
        newDevice = new GenericItem(cmnd->data.ad.pos(), cmnd->data.ad.id,
                                    cmnd->data.ad.typeItemId);
    }

    CommandViewManager::i()->scene()->addItem(newDevice);

    CommandViewManager::i()->devices().push_back(newDevice);

    return true;
}

bool CommandHandlerView::addConnection(std::shared_ptr<Command> cmnd) {
    auto source = CommandViewManager::i()->findItemWithId(cmnd->data.ac.sourceId);
    auto dest = CommandViewManager::i()->findItemWithId(cmnd->data.ac.destId);

    if (source == nullptr || dest == nullptr) {
        return false;
    }

    auto v1 = source->getOutput(cmnd->data.ac.sourceNum);
    auto v2 = dest->getInput(cmnd->data.ac.destNum);

    CommandViewManager::i()->scene()->addItem(new ConnectionItem(v1, v2));
    
    return true;
}

bool CommandHandlerView::removeItem(std::shared_ptr<Command> cmnd) {
    auto iter = std::find_if(CommandViewManager::i()->devices().begin(), CommandViewManager::i()->devices().end(),
                             [cmnd](ConstructorItem *item)
                             { return item->getId() == cmnd->data.dd.id; });

    if (iter == CommandViewManager::i()->devices().end()) {
        return false;
    }

    delete *iter;
    CommandViewManager::i()->devices().erase(iter);
    
    return true;
}

bool CommandHandlerView::removeConnection(std::shared_ptr<Command> cmnd) {
    auto vertex = CommandViewManager::i()->findItemWithId(cmnd->data.dc.sourceId);

    if (!vertex) {
        // что-то поломалось
        return false;
    }

    auto out = vertex->getOutput(cmnd->data.dc.sourceNum);

    delete out->getConnection();

    return true;
}

bool CommandHandlerView::changeVariables(std::shared_ptr<Command> cmnd) {
    auto device = CommandViewManager::i()->findItemWithId(cmnd->data.cv.id);

    if (!device) {
        return false;
    }

    QMap<QString, double> properties;

    for (const auto &i: cmnd->varList) {
        properties[i.first.c_str()] = i.second;
    }

    device->setProperties(properties);

    return true;
}
