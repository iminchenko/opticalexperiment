#include "commandhandlerchart.h"
#include "devicemanager.h"

CommandHandlerChart::CommandHandlerChart()
{

}

void CommandHandlerChart::setLayout(QLayout *layout) {
    _layout = layout;
}

bool CommandHandlerChart::handle(std::shared_ptr<Command> cmnd) {
    switch (cmnd->typeCommand) {
    case TypeCommand::CMND_ADD_DEVICE:
        if (cmnd->data.ad.typeItemId == deviceType::TYPE_SHIELD) {
            return createShield(cmnd);
        }
        return true;
    case TypeCommand::CMND_DELETE_DEVICE:
        if (findItemWithId(cmnd->data.dd.id)) {
             return removeShield(cmnd);
        }
    case TypeCommand::CMND_ADD_CONNECTION:
    case TypeCommand::CMND_REFRESH_DEVICE:
    case TypeCommand::CMND_DELETE_CONNECTION:
        return update();
    default:
        return true;
    }
}

bool CommandHandlerChart::createShield(std::shared_ptr<Command> cmnd){
    auto chart = new ChartView(cmnd->data.ad.id, _layout);
    chart->update(xMinus, xPlus, xPlus / sizeDiscretization, [](double x){return 0;});
    _charts.push_back(chart);
    return true;
}

bool CommandHandlerChart::removeShield(std::shared_ptr<Command> cmnd) {
    auto iter = _charts.begin();
    while (iter != _charts.end() && (*iter)->getId() != cmnd->data.dd.id) {
         iter++;
    }
    if(iter != _charts.end()) {
        delete (*iter);
        _charts.erase(iter);
    }
    return true;
}

bool CommandHandlerChart::update() {
    for(auto chart = _charts.begin(); chart != _charts.end(); chart++) {
        auto shield = dynamic_cast<Display*>(DEVICE_MANAGER.getDeviceById((*chart)->getId()).get());
        (*chart)->update(xMinus, xPlus, xPlus / sizeDiscretization,
                [&shield](double x){return shield->getValue(x).real();});
    }
    return true;
}

ChartView* CommandHandlerChart::findItemWithId(int id) {
    auto iter = _charts.begin();
    while (iter != _charts.end() && (*iter)->getId() != id) {
         iter++;
    }
    return *iter;
}
