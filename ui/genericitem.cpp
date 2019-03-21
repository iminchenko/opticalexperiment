#include "genericitem.h"
#include "deviceconfigs/deviceconfiglist.h"

GenericItem::GenericItem(QPointF pos,
                         int id,
                         int itemType,
                         QGraphicsItem *parent)
    :ConstructorItem(pos, id, parent), _itemId(itemType) {
    // как-то некрасиво, надо переписать
    int count = DEVICECONFIG_LIST[_itemId].getInputCount();
    if (count > 1) {
        for (int i = 0; i < count; ++i) {
            addInputVertex((float)i / (count - 1) * 90);
        }
    } else {
        addInputVertex();
    }

    count = DEVICECONFIG_LIST[_itemId].getOutputCount();

    addOutputVertex();
    for (int i = 1; i < count; ++i) {
        addOutputVertex(((float)i / (count - 1)) * 90);
    }

    const auto& vars =  DEVICECONFIG_LIST[_itemId].getVariables();
    QMap<QString, double> properties;

    for (const auto &iter : vars) {
        properties[QString(iter.first.c_str())] = iter.second;
    }

    setProperties(properties);
}

void GenericItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                        QWidget *) {
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::Antialiasing);

    DEVICECONFIG_LIST[_itemId].draw(painter, isSelected());
}

QRectF GenericItem::boundingRect() const {
    return DEVICECONFIG_LIST[_itemId].getBounding();
}
