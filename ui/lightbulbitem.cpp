#include <QPainter>

#include "lightbulbitem.h"

LightbulbItem::LightbulbItem(QPointF pos, QGraphicsItem *parent)
    :ConstructorItem(pos, parent) {
    addOutputVertex();
}

void LightbulbItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*,
                          QWidget*) {
    drawBox(painter);

    painter->setPen(QPen(getBorderColor(), getBorderWidth()));

    painter->drawArc(QRectF(-7, -12, 14, 14), -20 * 16, 220 * 16);

    painter->drawLine(QPointF(-6.5, -2), QPointF(-4, 4));
    painter->drawLine(QPointF(6.5, -2), QPointF(4, 4));
    painter->drawLine(QPointF(-4, 4), QPointF(4, 4));

    painter->drawLine(QPointF(-3.5, 8), QPointF(3.5, 8));
    painter->drawLine(QPointF(-2.5, 12), QPointF(2.5, 12));

    painter->drawLine(QPointF(0, 4), QPointF(0, -2));
    painter->drawLine(QPointF(-1.5, -4), QPointF(1.5, -4));
}
