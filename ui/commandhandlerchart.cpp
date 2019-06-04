#include "commandhandlerchart.h"
#include "devicemanager.h"
#include "ui/diffractiongratingchartitem.h"
#include "ui/shieldchartitem.h"
#include <QTabBar>

CommandHandlerChart::CommandHandlerChart()
    : CommandHandler(nullptr), Singleton<CommandHandlerChart>(*this) {
    _charts = new std::vector<std::shared_ptr<BaseChartItem>>();
}

CommandHandlerChart::~CommandHandlerChart() {
    delete _charts;
}

void CommandHandlerChart::setWidget(ChartWidget *chartWidget) {
    _chartWidget = chartWidget;
    _chartWidget->setChartsArray(_charts);
}

bool CommandHandlerChart::handle(std::shared_ptr<Command> cmnd) {
    switch (cmnd->typeCommand) {
    case TypeCommand::CMND_ADD_DEVICE:
        if (cmnd->data.ad.typeItemId == deviceType::TYPE_SHIELD) {
            return createShield(cmnd);
        }
        if (cmnd->data.ad.typeItemId == deviceType::TYPE_DIFFRACTION_GRATING) {
            return createDiffractionGrade(cmnd);
        }
        return true;
    case TypeCommand::CMND_DELETE_DEVICE:
        removeShield(cmnd);
        [[fallthrough]];
    case TypeCommand::CMND_ADD_CONNECTION:
    case TypeCommand::CMND_REFRESH_DEVICE:
    case TypeCommand::CMND_CHANGE_VARIABLE:
    case TypeCommand::CMND_DELETE_CONNECTION:
        update();
        return true;
    }
    return true;
}

bool CommandHandlerChart::createDiffractionGrade(
    std::shared_ptr<Command> cmnd) {
    auto shield = dynamic_cast<DiffractionGrating *>(
        DeviceManager::i()->getDeviceById(cmnd->data.ad.id).get());

    auto chart = std::shared_ptr<BaseChartItem>(
        new DiffractionGratingChartItem(cmnd->data.ad.id, shield));

    _charts->push_back(chart);
    _chartWidget->addChart(static_cast<int>(_charts->size() - 1));
    return true;
}

bool CommandHandlerChart::createShield(std::shared_ptr<Command> cmnd) {
    auto shield = dynamic_cast<Display *>(
        DeviceManager::i()->getDeviceById(cmnd->data.ad.id).get());

    auto chart = std::shared_ptr<BaseChartItem>(
        new ShieldChartItem(cmnd->data.ad.id, shield));

    _charts->push_back(chart);
    _chartWidget->addChart(static_cast<int>(_charts->size() - 1));
    return true;
}

bool CommandHandlerChart::removeShield(std::shared_ptr<Command> cmnd) {
    auto iter
        = std::find_if(_charts->begin(),
                       _charts->end(),
                       [cmnd](std::shared_ptr<BaseChartItem> item) -> bool {
                           return item->getId() == cmnd->data.dd.id;
                       });

    if (iter != _charts->end()) {
        _chartWidget->removeChart(iter - _charts->begin());
        _charts->erase(iter);
    }
    return true;
}

void CommandHandlerChart::update() {
    for (auto chart : *(_charts)) {
        chart->update();
    }
    _chartWidget->updateChart();
}
