#include <QPainter>
#include <cmath>

#include "shielditem.h"

ShieldItem::ShieldItem(QPointF pos,
                       int id,
                       QGraphicsItem *parent)
    :ConstructorItem(pos, id, parent) {
    addInputVertex();

    map properties;

    properties["deltaK"] = M_PI / 20;

    setProperties(properties);
}

void ShieldItem::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem*,
                       QWidget*) {
    drawBox(painter);

    painter->setPen(QPen(getBorderColor(), getBorderWidth()));
    painter->setBrush(Qt::NoBrush);

    painter->drawPolygon(QPolygonF() << QPointF(-5, -14) << QPointF(-5, 6)
                         << QPointF(5, 14) << QPointF(5, -6));
    painter->drawText(QPointF(-23, -14), "#"+QString::number(getId()));
}
