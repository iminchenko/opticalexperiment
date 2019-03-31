#include <QMouseEvent>
#include <memory>
#include <QDebug>

#include "constructareawidget.h"
#include "shielditem.h"
#include "laseritem.h"
#include "connectionitem.h"
#include "instrumentconfig.h"
#include "genericitem.h"
#include "command/command.h"
#include "command/commandhanlerglobal.h"
#include "utility/idgenerator.h"

ConstructAreaWidget::ConstructAreaWidget(QWidget *parent)
    :QGraphicsView(parent), _connectionLine(nullptr) {
    auto *scene = new QGraphicsScene(this);
    scene->setSceneRect(this->rect());
    setScene(scene);

    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::TextAntialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
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

    emit propertiesItemClicked(prop);
}

void ConstructAreaWidget::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);

    dropConnectionLine();
}

void ConstructAreaWidget::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);

    if (event->buttons()) {
        adjustConnectionLine(mapToScene(event->pos()));
        scene()->update();
    }
}

void ConstructAreaWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QGraphicsView::mouseDoubleClickEvent(event);

    invoke(Command::AddDevice(
               mapToScene(event->pos()), INSTRUMENT_CONFIG.getTypeId())
           );

    emit deviceEmplacementChanged();
}

void ConstructAreaWidget::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);

    if (event->key() == Qt::Key_Delete) {
        auto items = scene()->selectedItems();

        for (auto item : items) {
            auto device = dynamic_cast<ConstructorItem*>(item);
            if (device) {
                emit invoke(Command::DeleteDevice(device->getId()));
            } else {
                auto connection = dynamic_cast<ConnectionItem *>(item);
                if (connection) {
                    // TODO: убрать это страшилу
                    emit invoke(Command::DeleteConnection(
                            dynamic_cast<ConstructorItem *>(connection->getSource()->parentItem())->getId(),
                            dynamic_cast<ConstructorItem *>(connection->getDestination()->parentItem())->getId(),
                            connection->getSource()->getNumber(),
                            connection->getDestination()->getNumber()));
                }
            }
        }

        emit deviceEmplacementChanged();
    }

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
        if (v) {
            _connectionLine = scene()->addLine(QLineF(v->scenePos(), pos),
                                                           QPen(Qt::black, 0));
        }
    }
}

void ConstructAreaWidget::adjustConnectionLine(QPointF pos) {
    if (_connectionLine != nullptr) {
        _connectionLine->setLine(QLineF(_connectionLine->line().p1(), pos));
    }
}

void ConstructAreaWidget::dropConnectionLine() {
    if (_connectionLine != nullptr) {
        auto v1 = dynamic_cast<OutputVertexItem *>(scene()->itemAt(_connectionLine->line().p1(),
                                                                transform()));
        auto v2 = dynamic_cast<InputVertexItem *>(scene()->itemAt(_connectionLine->line().p2(),
                                                                transform()));

        if (v1 && v2) {
            auto source = dynamic_cast<ConstructorItem *>(v1->parentItem());
            auto dest = dynamic_cast<ConstructorItem *>(v2->parentItem());

            emit invoke(Command::AddConnection(source->getId(), dest->getId(),
                                               v1->getNumber(), v2->getNumber()));
        }

        delete _connectionLine;
        _connectionLine = nullptr;
    }

    emit deviceEmplacementChanged();
}
