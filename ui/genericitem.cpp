#include "genericitem.h"
#include "deviceconfigs/deviceconfiglist.h"

GenericItem::GenericItem(QPointF pos,
                         int id,
                         int itemType,
                         QGraphicsItem *parent)
    :ConstructorItem(pos, id, parent), _itemId(itemType) {
    // как-то некрасиво, надо переписать
    int count = (*DeviceConfigList::i())[_itemId].getInputCount();
    if (count > 1) {
        for (int i = 0; i < count; ++i) {
            addInputVertex(static_cast<float>(i) / (count - 1) * 90);
        }
    } else {
        addInputVertex();
    }

    count = (*DeviceConfigList::i())[_itemId].getOutputCount();

    addOutputVertex();
    for (int i = 1; i < count; ++i) {
        addOutputVertex((static_cast<float>(i) / (count - 1)) * 90);
    }

    const auto& vars =  (*DeviceConfigList::i())[_itemId].getVariables();
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

    (*DeviceConfigList::i())[_itemId].draw(painter, isSelected());
}

QRectF GenericItem::boundingRect() const {
    return (*DeviceConfigList::i())[_itemId].getBounding();
}
