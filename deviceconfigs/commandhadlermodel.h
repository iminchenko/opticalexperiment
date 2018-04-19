#ifndef COMANDHADLERMODEL_H
#define COMANDHADLERMODEL_H
#include <memory>

#include "command/commandhandler.h"
#include "utility/singleton.hpp"
#include "command/command.h"

#define CH_MODEL singleton<CommandHadlerModel>()

class CommandHadlerModel : CommandHandler {
    friend CommandHadlerModel& singleton<CommandHadlerModel>();

//ToDo: Не думаю, что так нужно делать. Мб переделать на friend class?
public:
    bool handle(std::shared_ptr<Command> cmnd) override;

private:
    bool addItem(std::shared_ptr<Command> cmnd);
    bool addConnection(std::shared_ptr<Command> cmnd);
    bool deleteItem();
    bool refreshItem();    
    
private:
    CommandHadlerModel();
     
};

#endif // COMANDHADLERMODEL_H
