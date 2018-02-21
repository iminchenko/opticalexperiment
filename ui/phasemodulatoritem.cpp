#include "phasemodulatoritem.h"

PhaseModulatorItem::PhaseModulatorItem(QPointF pos, QGraphicsItem *parent)
    :ConstructorItem(pos, parent) {
    addInputVertex();
    addOutputVertex();
}

void PhaseModulatorItem::paint(QPainter *painter,
                               const QStyleOptionGraphicsItem *, QWidget *) {
    drawBox(painter);

    painter->drawLine(-10, -5, 10, -5);
    painter->drawLine(-10, 5, 10, 5);

    painter->drawLine(0, -5, 0, -10);
    painter->drawLine(0, 5, 0, 10);
}
