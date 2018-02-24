#include "genericitem.h"
#include "deviceconfigs/deviceviewlist.h"

GenericItem::GenericItem(QPointF pos, int itemId, QGraphicsItem *parent)
    :ConstructorItem(pos, parent), _itemId(itemId) {
    // как-то некрасиво, надо переписать
    int count = DEVICEVIEW_LIST[_itemId].getInputCount();
    if (count > 1) {
        for (int i = 0; i < count; ++i)
            addInputVertex((float)i / (count - 1) * 90);
    }
    else {
        addInputVertex();
    }

    count = DEVICEVIEW_LIST[_itemId].getOutputCount();

    if (count > 1) {
        for (int i = 0; i < count; ++i)
            addOutputVertex(((float)i / (count - 1)) * 90);
    }
    else {
        addOutputVertex();
    }
}

void GenericItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                                                                    QWidget *) {
    DEVICEVIEW_LIST[_itemId].draw(painter, isSelected());
}

QRectF GenericItem::boundingRect() const {
    return DEVICEVIEW_LIST[_itemId].getBounding();
}
