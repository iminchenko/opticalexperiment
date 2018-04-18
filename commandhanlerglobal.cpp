#include "commandhanlerglobal.h"
#include "deviceconfigs/commandhadlermodel.h"
#include "ui/commandhandlerview.h"

CommandHanlerGlobal::CommandHanlerGlobal()
{
    
}

bool CommandHanlerGlobal::handler(std::shared_ptr<Command> cmnd)
{
    CH_MODEL.handler(cmnd);    
    CH_VIEW.handler(cmnd);  
    
}
