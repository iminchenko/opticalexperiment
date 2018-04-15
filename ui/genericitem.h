#pragma once

#include "constructoritem.h"

class GenericItem : public ConstructorItem {
public:
    GenericItem(QPointF pos, int id, int itemType, QGraphicsItem *parent = nullptr);

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    QRectF boundingRect() const override;

private:
    int _itemId;
};
