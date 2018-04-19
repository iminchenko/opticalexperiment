#include "commandhanlerglobal.h"
#include "deviceconfigs/commandhadlermodel.h"
#include "ui/commandhandlerview.h"

CommandHanlerGlobal::CommandHanlerGlobal()
{
    
}

bool CommandHanlerGlobal::handle(std::shared_ptr<Command> cmnd)
{
    CH_MODEL.handle(cmnd);
    CH_VIEW.handle(cmnd);
    
}
