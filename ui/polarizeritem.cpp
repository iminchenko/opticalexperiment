#include "polarizeritem.h"

PolarizerItem::PolarizerItem(QPointF pos, QGraphicsItem *parent)
    :ConstructorItem(pos, parent) {
    addInputVertex();
    addOutputVertex();
}

void PolarizerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                          QWidget *) {
    drawBox(painter);

    painter->drawRect(QRectF(QPointF(-10, -7), QPointF(10, 7)));
}
