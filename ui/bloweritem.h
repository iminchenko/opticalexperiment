#pragma once

#include "constructoritem.h"

class BlowerItem : public ConstructorItem {
public:
    BlowerItem(QPointF pos, QGraphicsItem *parent = nullptr);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
};
