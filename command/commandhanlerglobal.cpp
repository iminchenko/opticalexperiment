#include "commandhanlerglobal.h"
#include "deviceconfigs/commandhadlermodel.h"
#include "ui/commandhandlerview.h"
#include "ui/commandhandlerchart.h"

CommandHanlerGlobal::CommandHanlerGlobal()
    : CommandHandler(nullptr),
      Singleton<CommandHanlerGlobal>(*this) {}

bool CommandHanlerGlobal::handle(std::shared_ptr<Command> cmnd) {
    /* ToDo: Cпорное решение. Не уверен, что так должно быть */
    bool success = true;
    if(!CommandHadlerModel::i()->handle(cmnd)) {
        success = false;
        qDebug("Models handler failed!");
    }
    if(!CommandHandlerView::i()->handle(cmnd)) {
        success = false;
        qDebug("Views handler failed!");
    }
    if(!CommandHandlerChart::i()->handle(cmnd)) {
        success = false;
        qDebug("Charts handler failed!");
    }
    return success;
}
