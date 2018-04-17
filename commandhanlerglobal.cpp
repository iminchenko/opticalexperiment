#include "commandhanlerglobal.h"
#include "deviceconfigs/commandhadlermodel.h"
#include "ui/commandhandlerview.h"

CommandHanlerGlobal::CommandHanlerGlobal()
{
    
}

bool CommandHanlerGlobal::comandHandler(std::shared_ptr<Command> cmnd)
{
    switch (cmnd.get()->typeCommand()) {
    case TypeCommand::ADD: 
        // Обе функции возвращают bool. Надо будет что-нибудь из обработчиков.
        CH_MODEL.addItem(cmnd.get()->typeItemId(), cmnd.get()->id());    
        CH_VIEW.addItem(cmnd.get()->pos(), cmnd.get()->typeItem(),
                        cmnd.get()->typeItemId(), cmnd.get()->id());        
        break;
    case TypeCommand::DELETE:
        
        break;
    case TypeCommand::REFRESH:

        break;
    default:
        
        break;
    }
    
    return true;
}
