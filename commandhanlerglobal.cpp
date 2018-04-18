#include "commandhanlerglobal.h"
#include "deviceconfigs/commandhadlermodel.h"
#include "ui/commandhandlerview.h"

CommandHanlerGlobal::CommandHanlerGlobal()
{
    
}

bool CommandHanlerGlobal::handler(std::shared_ptr<Command> cmnd)
{
    switch (cmnd->typeCommand()) {
    case TypeCommand::CMND_ADD: 
        // Обе функции возвращают bool. Надо будет что-нибудь из обработчиков.
        CH_MODEL.addItem(cmnd);    
        CH_VIEW.addItem(cmnd);        
        break;
    case TypeCommand::CMND_DELETE:
        
        break;
    case TypeCommand::CMND_REFRESH:

        break;
    default:
        
        break;
    }
    
    return true;
}
