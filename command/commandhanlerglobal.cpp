#include "commandhanlerglobal.h"
#include "deviceconfigs/commandhadlermodel.h"
#include "ui/commandhandlerview.h"
#include "ui/commandhandlerchart.h"

CommandHanlerGlobal::CommandHanlerGlobal() {}

bool CommandHanlerGlobal::handle(std::shared_ptr<Command> cmnd) {
    /* ToDo: Cпорное решение. Не уверен, что так должно быть */
    bool success = true;
    if(!CH_MODEL.handle(cmnd)) {
        success = false;
        qDebug("Models handler failed!");
    }
    if(!CH_VIEW.handle(cmnd)) {
        success = false;
        qDebug("Views handler failed!");
    }
    if(!CH_CHART.handle(cmnd)) {
        success = false;
        qDebug("Charts handler failed!");
    }
    return success;
}
