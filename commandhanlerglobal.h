#ifndef COMANDHANLERGLOBAL_H
#define COMANDHANLERGLOBAL_H


#include "comandhandler.h"
#include "utility/singleton.hpp"

#define CH_GLOBAL singleton<CommandHanlerGlobal>()   

class CommandHanlerGlobal : CommandHandler
{
    friend CommandHanlerGlobal &singleton<CommandHanlerGlobal>();
public:
    CommandHanlerGlobal();
    
    bool comandHandler(std::shared_ptr<Command> cmnd) override;
};

#endif // COMANDHANLERGLOBAL_H
