#include <stdexcept>

#include "commandhandlerview.h"
#include "ui/commandviewmanager.h"

CommandHandlerView::CommandHandlerView() 
    : CommandHandler(nullptr),
      Singleton<CommandHandlerView>(*this) {}


//ToDo: Вполне возможно, что нужно перенести этот метод в CommandHandler
bool CommandHandlerView::handle(std::shared_ptr<Command> cmnd) {
    switch (cmnd->typeCommand) {
    case TypeCommand::CMND_ADD_DEVICE:
        return CommandViewManager::i()->addItem(cmnd);
    case TypeCommand::CMND_ADD_CONNECTION:
        return CommandViewManager::i()->addConnection(cmnd);
    case TypeCommand::CMND_DELETE_DEVICE:
        return CommandViewManager::i()->removeItem(cmnd);
    case TypeCommand::CMND_DELETE_CONNECTION:
        return CommandViewManager::i()->removeConnection(cmnd);
    case TypeCommand::CMND_CHANGE_VARIABLE:
        return CommandViewManager::i()->changeVariables(cmnd);
    case TypeCommand::CMND_REFRESH_DEVICE:
        return true;
    default:
        return true;
    }
}

