#include "bloweritem.h"

BlowerItem::BlowerItem(QPointF pos, QGraphicsItem *parent)
    :ConstructorItem(pos, parent) {
    addInputVertex(0);
    addInputVertex(90);
    addOutputVertex(0);
    addOutputVertex(90);
}

void BlowerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*,
                       QWidget *) {
    drawBox(painter);

    painter->setBrush(Qt::NoBrush);

    painter->drawLine(QPointF(15, 15), QPointF(-15, -15));
}
