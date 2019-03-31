#include "commandhandlerchart.h"
#include "devicemanager.h"
#include <QTabBar>
CommandHandlerChart::CommandHandlerChart() {}

void CommandHandlerChart::setTabWidget(QTabWidget *tabWidget) {
    _tabWidget = tabWidget;
}

bool CommandHandlerChart::handle(std::shared_ptr<Command> cmnd) {
    switch (cmnd->typeCommand) {
    case TypeCommand::CMND_ADD_DEVICE:
        if (cmnd->data.ad.typeItemId == deviceType::TYPE_SHIELD) {
            return createShield(cmnd);
        }
        return true;
    case TypeCommand::CMND_DELETE_DEVICE:
        return removeShield(cmnd);
    case TypeCommand::CMND_ADD_CONNECTION:
    case TypeCommand::CMND_REFRESH_DEVICE:
    case TypeCommand::CMND_CHANGE_VARIABLE:
    case TypeCommand::CMND_DELETE_CONNECTION:
        update();
        return true;
    default:
        return true;
    }
}

bool CommandHandlerChart::createShield(std::shared_ptr<Command> cmnd) {
    auto chart = std::make_shared<ChartView>(cmnd->data.ad.id, _tabWidget);
    chart->update(X_MINUS, X_PLUS, X_PLUS / SIZE_DISCRETIZATION, [](double){return 0;});
    _charts.push_back(chart);
    return true;
}

bool CommandHandlerChart::removeShield(std::shared_ptr<Command> cmnd) {
    auto iter = findIterWithId(cmnd->data.dd.id);

    if (iter != _charts.end()) {
        int tabIdx = (*iter)->getTabIndex();
        _charts.erase(iter);

        for (auto &chart : _charts) {
            chart->updateTabIndexAfterRemovingTab(tabIdx);
        }
    }

    return true;
}

void CommandHandlerChart::update() {
    for (auto &chart : _charts) {
        auto rawDevice = (DEVICE_MANAGER.getDeviceById(chart->getId()).get());
        auto shield = dynamic_cast<Display*>(rawDevice);
        chart->update(X_MINUS, X_PLUS, X_PLUS / SIZE_DISCRETIZATION,
                [&shield](double x){return shield->getValue(x).real();});
        chart->update3d([&shield](){return shield->getWave();});
    }
}

std::shared_ptr<ChartView> CommandHandlerChart::findItemWithId(int id) {
    return *findIterWithId(id);
}

QList<std::shared_ptr<ChartView>>::iterator CommandHandlerChart::findIterWithId(int id) {
    return std::find_if(_charts.begin(),
                        _charts.end(),
                        [id](auto cw) {
                            return cw->getId() == id;
                        });
}
