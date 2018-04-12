#include <QPainter>
#include <QKeyEvent>
#include <cmath>
#include <QGraphicsScene>

#include "constructoritem.h"

const QColor ConstructorItem::defaultBackgroundColor = QColor(200, 225, 255);
const QColor ConstructorItem::defaultSelectedColor = QColor(200, 255, 225);
const QColor ConstructorItem::defaultBorderColor = Qt::black;
const float ConstructorItem::defaultBorderWidth = 2;
const float ConstructorItem::defaultWidth = 50;

ConstructorItem::ConstructorItem(QPointF pos, int id, QGraphicsItem *parent)
    :QGraphicsItem(parent), _backgroundColor(defaultBackgroundColor),
      _selectedColor(defaultSelectedColor), _borderColor(defaultBorderColor),
      _borderWidth(defaultBorderWidth), _width(defaultWidth),
      _outCount(0), _inCount(0), _id(id) {
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemIsFocusable);

    setPos(pos);
    setZValue(1);
}

ConstructorItem::~ConstructorItem() {}

int ConstructorItem::getItemType() const {
    return (isSource() ? TYPE_SOURCE : 0) | (isReciever() ? TYPE_RECIEVER : 0);
}

bool ConstructorItem::isSource() const {
    return _source;
}

bool ConstructorItem::isReciever() const {
    return _reciever;
}

QColor ConstructorItem::getBackgroundColor() const {
    return _backgroundColor;
}

QColor ConstructorItem::getSelectedColor() const {
    return _selectedColor;
}

QColor ConstructorItem::getBorderColor() const {
    return _borderColor;
}

float ConstructorItem::getBorderWidth() const {
    return _borderWidth;
}

float ConstructorItem::getWidth() const {
    return _width;
}

int ConstructorItem::getId() const {
    return _id;
}

QRectF ConstructorItem::boundingRect() const {
    float halfWidth = _width/2;
    float halfBorderWidth = _borderWidth;
    return QRectF(-halfWidth - halfBorderWidth, -halfWidth - halfBorderWidth,
                  _width + _borderWidth, _width + _borderWidth);
}

void ConstructorItem::keyPressEvent(QKeyEvent *event) {
    QGraphicsItem::keyPressEvent(event);

//    // there may be a better solution
//    if (event->key() == Qt::Key_Delete)
//        delete this;
}

QVariant ConstructorItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                 const QVariant &value) {
    for (auto child : childItems()) {
        auto vertex = dynamic_cast<VertexItem*>(child);
        if (vertex && scene()) {
            scene()->update(vertex->connectionBoundingRect());
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void ConstructorItem::drawBox(QPainter *painter) const {
    painter->setPen(QPen(getBorderColor(), getBorderWidth()));

    if (!isSelected())
        painter->setBrush(getBackgroundColor());
    else
        painter->setBrush(getSelectedColor());

    float halfWidth = _width/2;
    painter->drawRoundRect(QRectF(-halfWidth, -halfWidth, _width, _width));
}

void ConstructorItem::addOutputVertex(float angle) {
    _source = true;

    auto vertex = new OutputVertexItem(this, _outCount++);
    vertex->setPos(0.75 * getWidth() * cos(angle*M_PI/180),
                   0.75 * getWidth() * sin(angle*M_PI/180));
    vertex->setRotation(angle);
}

void ConstructorItem::addInputVertex(float angle) {
    _reciever = true;

    auto vertex = new InputVertexItem(this, _inCount++);
    vertex->setPos(-0.75 * getWidth() * cos(angle*M_PI/180),
                   -0.75 * getWidth() * sin(angle*M_PI/180));
    vertex->setRotation(angle);
}
