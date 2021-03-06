#include <stdexcept>

#include "commandhandlerview.h"
#include "connectionitem.h"
#include "diffractiongratingitem.h"
#include "genericitem.h"
#include "laseritem.h"
#include "shielditem.h"

CommandHandlerView::CommandHandlerView()
    : CommandHandler(nullptr), Singleton<CommandHandlerView>(*this) {}

void CommandHandlerView::setScene(QGraphicsScene *scene) {
    _scene = scene;
}

// ToDo: Вполне возможно, что нужно перенести этот метод в CommandHandler
bool CommandHandlerView::handle(std::shared_ptr<Command> cmnd) {
    switch (cmnd->typeCommand) {
    case TypeCommand::CMND_ADD_DEVICE:
        return addItem(cmnd);
    case TypeCommand::CMND_ADD_CONNECTION:
        return addConnection(cmnd);
    case TypeCommand::CMND_DELETE_DEVICE:
        return removeItem(cmnd);
    case TypeCommand::CMND_DELETE_CONNECTION:
        return removeConnection(cmnd);
    case TypeCommand::CMND_CHANGE_VARIABLE:
        return changeVariables(cmnd);
    case TypeCommand::CMND_REFRESH_DEVICE:
        return true;
    }
    return true;
}

QPointF CommandHandlerView::getDevicePos(int id) {
    auto item = findItemWithId(id);

    if (!item) {
        throw std::logic_error("can't find device with id");
    }
    return item->pos();
}

bool CommandHandlerView::addItem(std::shared_ptr<Command> cmnd) {
    ConstructorItem *newDevice = nullptr;

    switch (cmnd->data.ad.typeItemId) {
    case deviceType::TYPE_LASER:
        newDevice = new LaserItem(cmnd->data.ad.pos(), cmnd->data.ad.id);
        break;
    case deviceType::TYPE_SHIELD:
        newDevice = new ShieldItem(cmnd->data.ad.pos(), cmnd->data.ad.id);
        break;
    case deviceType::TYPE_DIFFRACTION_GRATING:
        newDevice
            = new DiffractionGratingItem(cmnd->data.ad.pos(), cmnd->data.ad.id);
        break;
    default:
        newDevice = new GenericItem(
            cmnd->data.ad.pos(), cmnd->data.ad.id, cmnd->data.ad.typeItemId);
        break;
    }

    _scene->addItem(newDevice);
    _devices.push_back(newDevice);

    return true;
}

bool CommandHandlerView::addConnection(std::shared_ptr<Command> cmnd) {
    auto source = findItemWithId(cmnd->data.ac.sourceId);
    auto dest   = findItemWithId(cmnd->data.ac.destId);

    if (!source || !dest) {
        return false;
    }

    auto v1 = source->getOutput(cmnd->data.ac.sourceNum);
    auto v2 = dest->getInput(cmnd->data.ac.destNum);

    _scene->addItem(new ConnectionItem(v1, v2));

    /* ToDo: Точно ли так надо возвращать?*/
    return true;
}

bool CommandHandlerView::removeItem(std::shared_ptr<Command> cmnd) {
    auto iter = std::find_if(
        _devices.begin(), _devices.end(), [cmnd](ConstructorItem *item) {
            return item->getId() == cmnd->data.dd.id;
        });

    if (iter == _devices.end()) {
        return false;
    }

    delete *iter;
    _devices.erase(iter);

    return true;
}

bool CommandHandlerView::removeConnection(std::shared_ptr<Command> cmnd) {
    auto vertex = findItemWithId(cmnd->data.dc.sourceId);

    if (!vertex) {
        // что-то поломалось
        return false;
    }

    auto out = vertex->getOutput(cmnd->data.dc.sourceNum);

    delete out->getConnection();

    return true;
}

bool CommandHandlerView::changeVariables(std::shared_ptr<Command> cmnd) {
    auto device = findItemWithId(cmnd->data.cv.id);

    if (!device) {
        return false;
    }

    QMap<QString, double> properties;

    for (const auto &i : cmnd->varList) {
        properties[i.first.c_str()] = i.second;
    }

    device->setProperties(properties);

    return true;
}

ConstructorItem *CommandHandlerView::findItemWithId(int id) {
    auto iter = std::find_if(
        _devices.begin(), _devices.end(), [id](ConstructorItem *item) {
            return item->getId() == id;
        });

    return iter != _devices.end() ? *iter : nullptr;
}
