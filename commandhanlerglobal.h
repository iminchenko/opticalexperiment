#ifndef COMANDHANLERGLOBAL_H
#define COMANDHANLERGLOBAL_H


#include "commandhandler.h"
#include "utility/singleton.hpp"

#define CH_GLOBAL singleton<CommandHanlerGlobal>()   

class CommandHanlerGlobal : CommandHandler
{
    Q_OBJECT
    friend CommandHanlerGlobal &singleton<CommandHanlerGlobal>();
public:
    CommandHanlerGlobal();

public slots:
    bool handle(std::shared_ptr<Command> cmnd) override;
};

#endif // COMANDHANLERGLOBAL_H
