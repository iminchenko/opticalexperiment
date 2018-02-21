#include "laseritem.h"

LaserItem::LaserItem(QPointF pos, QGraphicsItem *parent)
    :ConstructorItem(pos, parent), _frequency(0) {
    addOutputVertex();
}

Properties::map LaserItem::getProperties() const {
    Properties::map prop;
    prop.insert("frequency", _frequency);
    return prop;
}

void LaserItem::setProperties(const Properties::map &properties) {
    if (properties.contains("frequency"))
        _frequency = properties["frequency"];
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
