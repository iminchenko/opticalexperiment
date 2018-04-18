#ifndef COMANDHADLERMODEL_H
#define COMANDHADLERMODEL_H
#include <memory>
#include "utility/singleton.hpp"
#include "command.h"

#define CH_MODEL singleton<CommandHadlerModel>()

class CommandHadlerModel {
    friend CommandHadlerModel& singleton<CommandHadlerModel>();

//ToDo: Не думаю, что так нужно делать. Мб переделать на friend class?
public:
    bool addItem(std::shared_ptr<Command> cmnd);
    bool deleteItem();
    bool refreshItem();    
    
private:
    CommandHadlerModel();
     
};

#endif // COMANDHADLERMODEL_H
