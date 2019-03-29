#ifndef COMANDHADLERMODEL_H
#define COMANDHADLERMODEL_H
#include <memory>

#include "command/commandhandler.h"
#include "utility/singleton.hpp"
#include "command/command.h"

#define CH_MODEL singleton<CommandHadlerModel>()

class CommandHadlerModel : CommandHandler {
    friend CommandHadlerModel& singleton<CommandHadlerModel>();

public:
    bool handle(std::shared_ptr<Command> cmnd) override;

private:
    bool addItem(pCommand cmnd);
    bool addConnection(pCommand cmnd);
    bool deleteItem(pCommand cmnd);
    bool deleteConnection(pCommand cmnd);
    bool changeVariables(pCommand cmnd);
    bool refreshItem(pCommand cmnd);
    
private:
    CommandHadlerModel();
     
};

#endif // COMANDHADLERMODEL_H
