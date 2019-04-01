#pragma once

#include "constructoritem.h"

class LaserItem : public ConstructorItem {
public:
    LaserItem(QPointF pos, int id, QGraphicsItem *parent = nullptr);

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};
