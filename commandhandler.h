#ifndef COMANDHANDLER_H
#define COMANDHANDLER_H

#include <QObject>
#include "command.h"
#include <memory>

// ToDo: Переименовать его в CommandHandlerBase или CommandHandlerInterface
class CommandHandler : public QObject
{
    Q_OBJECT
public:
    explicit CommandHandler(QObject *parent = nullptr);
    
signals:
    
public slots:
    virtual bool comandHandler(std::shared_ptr<Command> cmnd) = 0;
};

#endif // COMANDHANDLER_H
