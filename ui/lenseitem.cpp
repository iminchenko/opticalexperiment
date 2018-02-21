#include "lenseitem.h"

LenseItem::LenseItem(QPointF pos, QGraphicsItem *parent)
    :ConstructorItem(pos, parent) {
    addInputVertex();
    addOutputVertex();
}

void LenseItem::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *, QWidget *) {
    drawBox(painter);

    painter->setPen(QPen(defaultBorderColor, defaultBorderWidth));
    painter->setBrush(Qt::NoBrush);

    QPainterPath path;

    path.moveTo(0, -15);

    path.cubicTo(7, -3, 7, 3, 0, 15);
    path.cubicTo(-7, 3, -7, -3, 0, -15);

    painter->drawPath(path);
}
