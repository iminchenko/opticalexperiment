#pragma once

#include "constructoritem.h"

class LenseItem : public ConstructorItem {
public:
    LenseItem(QPointF pos, QGraphicsItem *parent = nullptr);

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};
