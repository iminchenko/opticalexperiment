#include <QPainter>

#include "opticalgratingitem.h"

OpticalGratingItem::OpticalGratingItem(QPointF pos, QGraphicsItem *parent)
    :ConstructorItem(pos, parent) {
    addInputVertex();
    addOutputVertex();
}

void OpticalGratingItem::paint(QPainter *painter,
                                    const QStyleOptionGraphicsItem*, QWidget*) {
    drawBox(painter);

    painter->setPen(QPen(getBorderColor(), getBorderWidth()));

    painter->setPen(QPen(Qt::black, 2, Qt::DashLine));

    painter->drawLine(0, -15, 0, 15);
}
