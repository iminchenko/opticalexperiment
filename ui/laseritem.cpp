#include "laseritem.h"

LaserItem::LaserItem(QPointF pos, int id, QGraphicsItem *parent)
    :ConstructorItem(pos, id, parent) {
    addOutputVertex();

    map properties;

    properties["Ex"] = 1;
    properties["Exi"] = 1;
    properties["Ey"] = 1;
    properties["Eyi"] = 1;

    setProperties(properties);
}

void LaserItem::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *, QWidget *) {
    drawBox(painter);

    painter->setPen(QPen(defaultBorderColor, defaultBorderWidth));
    painter->setBrush(Qt::NoBrush);

    painter->drawLine(QPointF(-15, 0), QPointF(15, 0));
    painter->drawLine(QPointF(10, 5), QPointF(10, -5));
    painter->drawLine(QPointF(7, 3), QPointF(13, -3));
    painter->drawLine(QPointF(13, 3), QPointF(7, -3));
}
