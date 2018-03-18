#include <QPainter>

#include "shielditem.h"

ShieldItem::ShieldItem(QPointF pos, int id, QGraphicsItem *parent)
    :ConstructorItem(pos, id, parent), _angle(0) {
    addInputVertex();
}

void ShieldItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*,
                          QWidget*) {
    drawBox(painter);

    painter->setPen(QPen(getBorderColor(), getBorderWidth()));
    painter->setBrush(Qt::NoBrush);

    painter->drawPolygon(QPolygonF() << QPointF(-5, -14) << QPointF(-5, 6)
                         << QPointF(5, 14) << QPointF(5, -6));
}

Properties::map ShieldItem::getProperties() const {
    Properties::map prop;
    prop.insert("angle", _angle);
    return prop;
}

void ShieldItem::setProperties(const Properties::map &properties) {
    if (properties.contains("angle"))
        _angle = properties["angle"];
}
