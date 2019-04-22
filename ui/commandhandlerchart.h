#pragma once

#include "command/command.h"
#include "command/commandhandler.h"
#include "deviceconfigs/devicedefines.h"
#include "ui/basechartitem.h"
#include "ui/chartwidget.h"
#include "utility/singleton.hpp"
#include <memory>

class CommandHandlerChart : CommandHandler,
                            public Singleton<CommandHandlerChart> {
public:
    CommandHandlerChart();
    ~CommandHandlerChart();
    bool handle(std::shared_ptr<Command> cmnd) override;
    void setWidget(ChartWidget *chartWidget);

private:
    std::shared_ptr<BaseChartItem> findItemWithId(int id);
    QList<std::shared_ptr<BaseChartItem>>::iterator findIterWithId(int id);

    bool createDiffractionGrade(std::shared_ptr<Command> cmnd);
    bool createShield(std::shared_ptr<Command> cmnd);

    bool removeShield(std::shared_ptr<Command> cmnd);
    void update();

    ChartWidget *_chartWidget = nullptr;
    std::vector<std::shared_ptr<BaseChartItem>> *_charts;
};
