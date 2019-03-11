#ifndef COMMANDHANDLERCHART_H
#define COMMANDHANDLERCHART_H

#include <memory>

#include "command/commandhandler.h"
#include "ui/chartview.h"
#include "utility/singleton.hpp"
#include "deviceconfigs/devicedefines.h"
#include "command/command.h"

#define CH_CHART singleton<CommandHandlerChart>()

class CommandHandlerChart: CommandHandler {
    friend CommandHandlerChart& singleton<CommandHandlerChart>();

public:
    void setTabWidget(QTabWidget *tabWidget);
    bool handle(std::shared_ptr<Command> cmnd) override;

private:
    CommandHandlerChart();

    ChartView *findItemWithId(int id);

    bool createShield(std::shared_ptr<Command> cmnd);
    bool removeShield(std::shared_ptr<Command> cmnd);
    bool update();

    QTabWidget* _tabWidget;
    QList<ChartView *> _charts;
};

#endif // COMMANDHANDLERCHART_H
