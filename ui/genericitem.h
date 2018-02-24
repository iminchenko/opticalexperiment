#pragma once

#include "constructoritem.h"

class GenericItem : public ConstructorItem {
public:
    GenericItem(QPointF pos, int itemId, QGraphicsItem *parent = nullptr);

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual QRectF boundingRect() const override;

private:
    int _itemId;
};
