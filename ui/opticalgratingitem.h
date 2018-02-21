#pragma once

#include "constructoritem.h"

class OpticalGratingItem : public ConstructorItem {
public:
    OpticalGratingItem(QPointF pos, QGraphicsItem *parent = nullptr);

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};
