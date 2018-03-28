#include "genericitem.h"
#include "deviceconfigs/deviceconfiglist.h"

GenericItem::GenericItem(QPointF pos, int id, int itemType,
                         QGraphicsItem *parent)
    :ConstructorItem(pos, id, parent), _itemId(itemType) {
    // как-то некрасиво, надо переписать
    int count = DEVICECONFIG_LIST[_itemId].getInputCount();
    if (count > 1) {
        for (int i = 0; i < count; ++i)
            addInputVertex((float)i / (count - 1) * 90);
    }
    else {
        addInputVertex();
    }

    count = DEVICECONFIG_LIST[_itemId].getOutputCount();

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
    DEVICECONFIG_LIST[_itemId].draw(painter, isSelected());
}

QRectF GenericItem::boundingRect() const {
    return DEVICECONFIG_LIST[_itemId].getBounding();
}
