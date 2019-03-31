#ifndef COMMANDHANDLERCHART_H
#define COMMANDHANDLERCHART_H

#include <memory>

#include "command/commandhandler.h"
#include "ui/chartview.h"
#include "utility/singleton.hpp"
#include "deviceconfigs/devicedefines.h"
#include "command/command.h"

class CommandHandlerChart: CommandHandler, public Singleton<CommandHandlerChart> {
public:
    CommandHandlerChart();
    
    void setTabWidget(QTabWidget *tabWidget);
    bool handle(std::shared_ptr<Command> cmnd) override;

private:
    std::shared_ptr<ChartView> findItemWithId(int id);
    QList<std::shared_ptr<ChartView>>::iterator findIterWithId(int id);

    bool createShield(std::shared_ptr<Command> cmnd);
    bool removeShield(std::shared_ptr<Command> cmnd);
    void update();

    QTabWidget* _tabWidget;
    QList<std::shared_ptr<ChartView>> _charts;
};

#endif // COMMANDHANDLERCHART_H
