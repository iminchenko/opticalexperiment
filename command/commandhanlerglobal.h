#ifndef COMANDHANLERGLOBAL_H
#define COMANDHANLERGLOBAL_H


#include "command/commandhandler.h"
#include "utility/singleton.hpp"

#define CH_GLOBAL singleton<CommandHanlerGlobal>()   

class CommandHanlerGlobal : public CommandHandler {
    friend CommandHanlerGlobal &singleton<CommandHanlerGlobal>();
public:
Q_OBJECT
    CommandHanlerGlobal();

public slots:
    bool handle(std::shared_ptr<Command> cmnd) override;
};

#endif // COMANDHANLERGLOBAL_H
