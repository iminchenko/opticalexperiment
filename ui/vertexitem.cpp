#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>

#include "constructoritem.h"
#include "vertexitem.h"
#include "connectionitem.h"

using std::list;

VertexItem::VertexItem(ConstructorItem *parent, int number)
    :QGraphicsItem(parent), _hoverEvent(false),
     _connection(nullptr), _number(number){
    setZValue(1);
    setAcceptHoverEvents(true);
}

VertexItem::~VertexItem() {
    delete _connection;
}

bool VertexItem::canConnect(const VertexItem *vertex) {
    if (_connection && (_connection->getDestination() == vertex
                        || _connection->getSource() == vertex)) {
        return false;
    }

    if (vertex->parentItem() == parentItem()) {
        return false;
    }

    return !_connection;
}

int VertexItem::getNumber() const {
    return _number;
}

bool VertexItem::isHoverEvent() const {
    return _hoverEvent;
}

QRectF VertexItem::boundingRect() const {
    return QRectF(-10, -10, 20, 20);
}

QRectF VertexItem::connectionBoundingRect() const {
    return _connection ? _connection->boundingRect() : QRectF();
}

void VertexItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsItem::hoverEnterEvent(event);
    _hoverEvent = true;
}

void VertexItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsItem::hoverLeaveEvent(event);
    _hoverEvent = false;
}

InputVertexItem::InputVertexItem(ConstructorItem *parent, int number)
    :VertexItem(parent, number) {}

void InputVertexItem::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem*,
                            QWidget*) {
    if (isHoverEvent()) {
        drawAim(painter);
    }

    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(QColor(200, 225, 255));

    painter->drawEllipse(-4, -4, 8, 8);
}

void VertexItem::drawAim(QPainter *painter) const {
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::NoBrush);

    painter->drawRoundedRect(QRectF(-9, -9, 18, 18), 5, 5);
}

bool VertexItem::addConnection(ConnectionItem *connection) {
   if (_connection) {
       return false;
   }

   _connection = connection;
   return true;
}

void VertexItem::removeConnection() {
    _connection = nullptr;
}

QVariant VertexItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                const QVariant &value) {
    if (_connection) {
        _connection->update();
    }
    return QGraphicsItem::itemChange(change, value);
}

ConnectionItem *VertexItem::getConnection() {
    return _connection;
}

OutputVertexItem::OutputVertexItem(ConstructorItem *parent, int number)
    :VertexItem(parent, number) {}

void OutputVertexItem::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem*,
                             QWidget*) {
    if (isHoverEvent()) {
        drawAim(painter);
    }

    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(QColor(200, 225, 255));

    painter->drawPolygon(QPolygonF() << QPointF(-4, 5) << QPointF(-4, -5)
                        << QPointF(4, 0));
}
