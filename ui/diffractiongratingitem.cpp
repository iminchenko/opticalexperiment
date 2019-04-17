#include "diffractiongratingitem.h"
#include <cmath>

DiffractionGratingItem::DiffractionGratingItem(QPointF pos, int id, QGraphicsItem *parent)
    : ShieldItem(pos, id, parent)
{
    map properties;

    properties["a"] = 0.00001;
    properties["b"] = 0.000001;
    properties["W"] = 1;
    properties["N3"] = 1;
    properties["deltaK"] = M_PI / 20;

    setProperties(properties);
}

void DiffractionGratingItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    drawBox(painter);

    painter->setPen(QPen(getBorderColor(), getBorderWidth()));
    painter->setBrush(Qt::NoBrush);

    painter->drawPolygon(QPolygonF() << QPointF(-5, -14) << QPointF(-5, 6)
                         << QPointF(5, 14) << QPointF(5, -6));
    painter->drawLine(-5,2,5,10);
    painter->drawLine(-5,-2,5,6);
    painter->drawLine(-5,-6,5,2);
    painter->drawLine(-5,-10,5,-2);
    painter->drawText(QPointF(-23, -14), "#"+QString::number(getId()));
}




