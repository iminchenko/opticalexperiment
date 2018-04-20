#pragma once

#include "constructoritem.h"

class ShieldItem : public ConstructorItem {
public:
    ShieldItem(QPointF pos, int id, QGraphicsItem *parent = nullptr);

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};
