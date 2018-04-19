#ifndef COMANDHANDLER_H
#define COMANDHANDLER_H

#include <QObject>
#include "command/command.h"
#include <memory>

// ToDo: Переименовать его в CommandHandlerBase или CommandHandlerInterface
class CommandHandler : public QObject
{
    Q_OBJECT
public:
    explicit CommandHandler(QObject *parent = nullptr);
    
signals:
    
public slots:
    virtual bool handle(std::shared_ptr<Command> cmnd) = 0;
};

#endif // COMANDHANDLER_H
