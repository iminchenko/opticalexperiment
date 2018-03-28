#include <QMouseEvent>

#include "constructareawidget.h"
#include "shielditem.h"
#include "laseritem.h"
#include "connectionitem.h"
#include "instrumentconfig.h"
#include "genericitem.h"
#include "devicemanager.h"

ConstructAreaWidget::ConstructAreaWidget(QWidget *parent)
    :QGraphicsView(parent), _connectionLine(nullptr) {
    auto *scene = new QGraphicsScene(this);
    scene->setSceneRect(this->rect());
    setScene(scene);

    setRenderHint(QPainter::Antialiasing);
//    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void ConstructAreaWidget::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);

    if (event->button() == Qt::RightButton) {
         createConnectionLine(mapToScene(event->pos()));
    }

    // выглядит как то, чего тут не должно быть
    auto prop =
            dynamic_cast<Properties *>(scene()->itemAt(mapToScene(event->pos()),
                                                       transform()));
    // сигнал с nullptr - это ок
    emit propertiesItemClicked(prop);
}

void ConstructAreaWidget::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);

    dropConnectionLine();
}

void ConstructAreaWidget::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);

    adjustConnectionLine(mapToScene(event->pos()));
}

void ConstructAreaWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QGraphicsView::mouseDoubleClickEvent(event);

    auto pos = mapToScene(event->pos());

    int type;
    int id = DEVICE_MANAGER.getMaxId();

    switch (INSTRUMENT_CONFIG.getType()) {
    case deviceType::TYPE_LASER:
        scene()->addItem(new LaserItem(pos, id));
        type = deviceType::TYPE_LASER;
        break;
    case deviceType::TYPE_SHIELD:
        scene()->addItem(new ShieldItem(pos, id));
        type = deviceType::TYPE_SHIELD;
        break;
    case deviceType::TYPE_GENERIC:
        scene()->addItem(new GenericItem(pos, id,
                                         INSTRUMENT_CONFIG.getTypeId()));
        type = INSTRUMENT_CONFIG.getTypeId();
        break;
    }

    DEVICE_MANAGER.addDevice(type, id);
}

void ConstructAreaWidget::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
    // отмена выбранного объекта со свойствами
    emit propertiesItemClicked(nullptr);
}

void ConstructAreaWidget::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    scene()->setSceneRect(0, 0, event->size().width(), event->size().height());
}

void ConstructAreaWidget::createConnectionLine(QPointF pos) {
    if (_connectionLine == nullptr) {
        auto v = dynamic_cast<OutputVertexItem *>(scene()->itemAt(pos,
                                                                  transform()));
        if (v)
            _connectionLine = scene()->addLine(QLineF(v->scenePos(), pos),
                                                           QPen(Qt::black, 0));
    }
}

void ConstructAreaWidget::adjustConnectionLine(QPointF pos) {
    if (_connectionLine != nullptr)
        _connectionLine->setLine(QLineF(_connectionLine->line().p1(), pos));
}

void ConstructAreaWidget::dropConnectionLine() {
    if (_connectionLine != nullptr) {
        auto v1 = dynamic_cast<OutputVertexItem *>(scene()->itemAt(_connectionLine->line().p1(),
                                                                transform()));
        auto v2 = dynamic_cast<InputVertexItem *>(scene()->itemAt(_connectionLine->line().p2(),
                                                                transform()));

        if (v1 && v2 && v1->canConnect(v2)) {
            scene()->addItem(new ConnectionItem(v1, v2));
            auto source = dynamic_cast<ConstructorItem *>(v1->parentItem());
            auto dest = dynamic_cast<ConstructorItem *>(v2->parentItem());
            // нет проверки на нулевые указатели
            DEVICE_MANAGER.addConnection(source->getId(), v1->getNumber(),
                                         dest->getId(), v2->getNumber());
        }

        delete _connectionLine;
        _connectionLine = nullptr;
    }
}
