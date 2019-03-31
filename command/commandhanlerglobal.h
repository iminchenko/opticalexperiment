#ifndef COMANDHANLERGLOBAL_H
#define COMANDHANLERGLOBAL_H

#include "command/commandhandler.h"
#include "utility/singleton.hpp"

class CommandHanlerGlobal : public CommandHandler, public Singleton<CommandHanlerGlobal> {
    Q_OBJECT
public:
    CommandHanlerGlobal();

public slots:
    bool handle(std::shared_ptr<Command> cmnd) override;
};

#endif // COMANDHANLERGLOBAL_H
