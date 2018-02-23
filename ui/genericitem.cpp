#include "genericitem.h"
#include "deviceconfigs/deviceviewlist.h"

GenericItem::GenericItem(QPointF pos, int itemId, QGraphicsItem *parent)
    :ConstructorItem(pos, parent), _itemId(itemId) {}

void GenericItem::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option, QWidget *widget) {
//    drawBox(painter);

//    painter->setPen(QPen(defaultBorderColor, defaultBorderWidth));
//    painter->setBrush(Qt::NoBrush);

//    painter->drawLine(QPointF(-15, 0), QPointF(15, 0));
//    painter->drawLine(QPointF(10, 5), QPointF(10, -5));
//    painter->drawLine(QPointF(7, 3), QPointF(13, -3));
//    painter->drawLine(QPointF(13, 3), QPointF(7, -3));

    DEVICEVIEW_LIST[_itemId].draw(painter);
}
