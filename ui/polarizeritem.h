#pragma once

#include "constructoritem.h"

class PolarizerItem : public ConstructorItem {
public:
    PolarizerItem(QPointF pos, QGraphicsItem *parent = nullptr);

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
};
